#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include "sm_drv_types.h"
#include "sram.h"
#include "sram_ioctl.h"
#include "application_cfg.h" /* Configuration macros for ioctl from the SM streaming application */
#include "eth_queue_cfg.h"   /* Configuration of the Rx queue from the SM streaming application */

/* Used drivers */
#define SRAM_DRV    "/dev/sram"
#define SM_DRV      "/dev/sm_drv"

/* Used streams */
#define CFG_STREAM_NUM  1U
#define CFG_STREAM0_ID  19024ULL
#define CFG_STREAM1_ID  19025ULL
#define CFG_STREAM2_ID  19026ULL
#define CFG_STREAM3_ID  19027ULL

/* Definition of the video frame fields */
#define ETH_FRM_OFFSET      2U
#define ETH_HEAD_LEN        14U
#define VLAN_LEN            4U
#define AVB_OFFSET          (ETH_FRM_OFFSET+ETH_HEAD_LEN+VLAN_LEN)
#define AVB_HEAD_LEN        24U
#define JPEG_HEAD_OFFSET    (AVB_OFFSET+AVB_HEAD_LEN)
#define JPEG_HEAD_LEN       8U
#define CFG_SEQID_OFFSET    (AVB_OFFSET+2U)
#define CFG_STREAMID_OFFSET (AVB_OFFSET+4U)
#define CFG_SOI_MARK_VALUE  ((uint32_t)0x000000U)
#define CFG_SOI_MARK_MASK   ((uint32_t)0xFFFFFFU)
#define CFG_SOI_MARK_OFFSET (JPEG_HEAD_OFFSET+0U)
#define CFG_FRM_DATA_OFFSET (JPEG_HEAD_OFFSET+JPEG_HEAD_LEN)

/* Specify VLAN priorities used by video streams (1 to 4 needed) */
#define CFG_VLAN_TAG_COUNT      1U
#define CFG_VLAN_TAG0           0U
#define CFG_VLAN_TAG1           0U
#define CFG_VLAN_TAG2           0U
#define CFG_VLAN_TAG3           0U
/* Each TAG occupies one nibble. The most significant bit states whether the nibble is used. */
#define CFG_VLAN_TAGS           ( (( ((CFG_VLAN_TAG_COUNT>3U)?8U:0U) | CFG_VLAN_TAG3 ) << 12U) | \
                                  (( ((CFG_VLAN_TAG_COUNT>2U)?8U:0U) | CFG_VLAN_TAG2 ) << 8U) | \
      /* DO NOT CHANGE */         (( ((CFG_VLAN_TAG_COUNT>1U)?8U:0U) | CFG_VLAN_TAG1 ) << 4U) | \
      /* THIS MACRO    */         (( ((CFG_VLAN_TAG_COUNT>0U)?8U:0U) | CFG_VLAN_TAG0 ) << 0U) \
                                )

#define CFG_FRAME_BUFFER_LENGTH 1536U /* Minimum is AVB_OFFSET+1500 */ /* FIXME send this value to the sm_app */


/*
    This is VideoListener Demo application.

    Preconditions:
        - the sm_drv must be loaded
        - the sram driver must be loaded
        - modified FEC driver which leaves us queue number 1  must be loaded
*/

static void sig_action(int sig, siginfo_t *siginfo, void *context) {
    if (SIGPOLL == sig) {
        printf("[app] SIGPOLL: ");
        switch (siginfo->si_code) {
            case POLL_IN:  printf("POLL_IN");  break;
            case POLL_OUT: printf("POLL_OUT"); break;
            case POLL_MSG: printf("POLL_MSG"); break;
            case POLL_ERR: printf("POLL_ERR"); break;
            case POLL_PRI: printf("POLL_PRI"); break;
            case POLL_HUP: printf("POLL_HUP"); break;
            default: printf("UNKNOWN CODE");  break;
        }
        /*  FIXME: do more than just print */
        if(0U == (siginfo->si_int & 0x80000000U)) /* Bit clear -> StreamCore error mask */
            printf(" stream_core error, ");
        else /* Bit set -> SM app error mask */
            printf(" sm_app error, ");
        printf("error mask: 0x%x\n", (uint32_t)(siginfo->si_int));
    }
    else
        printf("[app] Signal action executed: %d\n", sig);
}

int main(int argc, char **argv) {
    int fd_sram, fd_sm, ret;
    sm_drv_sc_param_t sc_prm;
    unsigned long sralloc_prm, EthBufPtr = 0, EthBufLen = 0;
    pid_t mypid;
    struct sigaction sig_act;

    /*  Open the sram allocator */
    fd_sram = open(SRAM_DRV, 0);
    if (fd_sram < 0) {
        printf("[app] Unable to open device %s: %s\n", SRAM_DRV, strerror(errno));
        return EXIT_FAILURE;
    }

    /*  Allocate memory for our ENET Rx queue buffers */
    sralloc_prm = (CFG_FRAME_BUFFER_LENGTH * ETHQ_CFG_RX_BD_RING_LEN);
    EthBufLen = sralloc_prm;
    ret = ioctl(fd_sram, IOCTL_SRAM_S_MALLOC, &sralloc_prm);
    if (ret) {
        printf("[app] Memory allocation failed: %d\n", ret);
        exit(-1);
    }
    else {
        printf("[app] SRAM granted @0x%lx\n", sralloc_prm);
        EthBufPtr = sralloc_prm;
    }

    /*  Open the sm_drv */
    fd_sm = open(SM_DRV, 0);
    if (fd_sm < 0) {
        printf("[app] Unable to open device %s: %s\n", SM_DRV, strerror(errno));
        return EXIT_FAILURE;
    }

    /*  Configure the SM app */
    /*===== PIT/FIQ period =====*/
    /*  Prepare new parameter value */
    sc_prm.s64_val0 = 0x1;            /*  Key     : APP_KEY_CHANGE_PIT_F   */
    sc_prm.s64_val1 = 1666666;        /*  Value   : New PIT period [ns] (~600 Hz) */
    /* Send "set cfg" commands */
    if (ret = ioctl(fd_sm, SM_DRV_IOCTL_SET_CFG, (long)&sc_prm))    { printf("[app] SM_DRV_IOCTL_SET_CFG failed: %s\n", strerror(errno)); exit(ret); }
    /* Check also the value returned by called SM app function */
    if( 0 != sc_prm.s64_val0 )  { printf("[app] SM app configuration callback failed: %d\n", sc_prm.s64_val0); exit(-1); }
    /*===== Streaming parameters =====*/
    sc_prm.s64_val0 = APP_KEY_STRM_FETCH_THRESHOLD;    /*  Key  */
    sc_prm.s64_val1 = 120U;    /*  Value  */
    if (ret = ioctl(fd_sm, SM_DRV_IOCTL_SET_CFG, (long)&sc_prm))    { printf("[app] SM_DRV_IOCTL_SET_CFG failed: %s\n", strerror(errno)); exit(ret); }
    if( 0 != sc_prm.s64_val0 )  { printf("[app] SM app configuration of Fetch Threshold has failed: %d\n", sc_prm.s64_val0); exit(-1); }
    sc_prm.s64_val0 = APP_KEY_STRM_SOI_MARK_VALUE;    /*  Key  */
    sc_prm.s64_val1 = CFG_SOI_MARK_VALUE;    /*  Value  */
    if (ret = ioctl(fd_sm, SM_DRV_IOCTL_SET_CFG, (long)&sc_prm))    { printf("[app] SM_DRV_IOCTL_SET_CFG failed: %s\n", strerror(errno)); exit(ret); }
    if( 0 != sc_prm.s64_val0 )  { printf("[app] SM app configuration of SOI marker value has failed: %d\n", sc_prm.s64_val0); exit(-1); }
    sc_prm.s64_val0 = APP_KEY_STRM_SOI_MARK_OFFSET;    /*  Key  */
    sc_prm.s64_val1 = CFG_SOI_MARK_OFFSET;    /*  Value  */
    if (ret = ioctl(fd_sm, SM_DRV_IOCTL_SET_CFG, (long)&sc_prm))    { printf("[app] SM_DRV_IOCTL_SET_CFG failed: %s\n", strerror(errno)); exit(ret); }
    if( 0 != sc_prm.s64_val0 )  { printf("[app] SM app configuration of SOI marker offset has failed: %d\n", sc_prm.s64_val0); exit(-1); }
    sc_prm.s64_val0 = APP_KEY_STRM_SOI_MARK_MASK;    /*  Key  */
    sc_prm.s64_val1 = CFG_SOI_MARK_MASK;    /*  Value  */
    if (ret = ioctl(fd_sm, SM_DRV_IOCTL_SET_CFG, (long)&sc_prm))    { printf("[app] SM_DRV_IOCTL_SET_CFG failed: %s\n", strerror(errno)); exit(ret); }
    if( 0 != sc_prm.s64_val0 )  { printf("[app] SM app configuration of SOI marker mask has failed: %d\n", sc_prm.s64_val0); exit(-1); }
    sc_prm.s64_val0 = APP_KEY_STRM_SEQ_NUM_OFFSET;    /*  Key  */
    sc_prm.s64_val1 = CFG_SEQID_OFFSET;    /*  Value  */
    if (ret = ioctl(fd_sm, SM_DRV_IOCTL_SET_CFG, (long)&sc_prm))    { printf("[app] SM_DRV_IOCTL_SET_CFG failed: %s\n", strerror(errno)); exit(ret); }
    if( 0 != sc_prm.s64_val0 )  { printf("[app] SM app configuration of Sequence ID offset has failed: %d\n", sc_prm.s64_val0); exit(-1); }
    sc_prm.s64_val0 = APP_KEY_STRM_STRM_ID_OFFSET;    /*  Key  */
    sc_prm.s64_val1 = CFG_STREAMID_OFFSET;    /*  Value  */
    if (ret = ioctl(fd_sm, SM_DRV_IOCTL_SET_CFG, (long)&sc_prm))    { printf("[app] SM_DRV_IOCTL_SET_CFG failed: %s\n", strerror(errno)); exit(ret); }
    if( 0 != sc_prm.s64_val0 )  { printf("[app] SM app configuration of Stream ID offset has failed: %d\n", sc_prm.s64_val0); exit(-1); }
    sc_prm.s64_val0 = APP_KEY_STRM_NUMBER_OF_STRMS;    /*  Key  */
    sc_prm.s64_val1 = CFG_STREAM_NUM;    /*  Value  */
    if (ret = ioctl(fd_sm, SM_DRV_IOCTL_SET_CFG, (long)&sc_prm))    { printf("[app] SM_DRV_IOCTL_SET_CFG failed: %s\n", strerror(errno)); exit(ret); }
    if( 0 != sc_prm.s64_val0 )  { printf("[app] SM app configuration of number of streams has failed: %d\n", sc_prm.s64_val0); exit(-1); }
    sc_prm.s64_val0 = APP_KEY_STRM_STREAM_ID_0;    /*  Key  */
    sc_prm.s64_val1 = CFG_STREAM0_ID;    /*  Value  */
    if (ret = ioctl(fd_sm, SM_DRV_IOCTL_SET_CFG, (long)&sc_prm))    { printf("[app] SM_DRV_IOCTL_SET_CFG failed: %s\n", strerror(errno)); exit(ret); }
    if( 0 != sc_prm.s64_val0 )  { printf("[app] SM app configuration of StreamID[0] has failed: %d\n", sc_prm.s64_val0); exit(-1); }
    sc_prm.s64_val0 = APP_KEY_STRM_STREAM_ID_1;    /*  Key  */
    sc_prm.s64_val1 = CFG_STREAM1_ID;    /*  Value  */
    if (ret = ioctl(fd_sm, SM_DRV_IOCTL_SET_CFG, (long)&sc_prm))    { printf("[app] SM_DRV_IOCTL_SET_CFG failed: %s\n", strerror(errno)); exit(ret); }
    if( 0 != sc_prm.s64_val0 )  { printf("[app] SM app configuration of StreamID[1] has failed: %d\n", sc_prm.s64_val0); exit(-1); }
    sc_prm.s64_val0 = APP_KEY_STRM_STREAM_ID_2;    /*  Key  */
    sc_prm.s64_val1 = CFG_STREAM2_ID;    /*  Value  */
    if (ret = ioctl(fd_sm, SM_DRV_IOCTL_SET_CFG, (long)&sc_prm))    { printf("[app] SM_DRV_IOCTL_SET_CFG failed: %s\n", strerror(errno)); exit(ret); }
    if( 0 != sc_prm.s64_val0 )  { printf("[app] SM app configuration of StreamID[2] has failed: %d\n", sc_prm.s64_val0); exit(-1); }
    sc_prm.s64_val0 = APP_KEY_STRM_STREAM_ID_3;    /*  Key  */
    sc_prm.s64_val1 = CFG_STREAM3_ID;    /*  Value  */
    if (ret = ioctl(fd_sm, SM_DRV_IOCTL_SET_CFG, (long)&sc_prm))    { printf("[app] SM_DRV_IOCTL_SET_CFG failed: %s\n", strerror(errno)); exit(ret); }
    if( 0 != sc_prm.s64_val0 )  { printf("[app] SM app configuration of StreamID[3] has failed: %d\n", sc_prm.s64_val0); exit(-1); }
    sc_prm.s64_val0 = APP_KEY_STRM_FRM_DATA_OFFSET;    /*  Key  */
    sc_prm.s64_val1 = CFG_FRM_DATA_OFFSET;    /*  Value  */
    if (ret = ioctl(fd_sm, SM_DRV_IOCTL_SET_CFG, (long)&sc_prm))    { printf("[app] SM_DRV_IOCTL_SET_CFG failed: %s\n", strerror(errno)); exit(ret); }
    if( 0 != sc_prm.s64_val0 )  { printf("[app] SM app configuration of Eth frame data offset has failed: %d\n", sc_prm.s64_val0); exit(-1); }
    /*===== Ethernet Queue parameters =====*/
    sc_prm.s64_val0 = APP_KEY_ETHQ_SIZE_OF_BUFF;    /*  Key  */
    sc_prm.s64_val1 = EthBufLen;    /*  Value  */
    if (ret = ioctl(fd_sm, SM_DRV_IOCTL_SET_CFG, (long)&sc_prm))    { printf("[app] SM_DRV_IOCTL_SET_CFG failed: %s\n", strerror(errno)); exit(ret); }
    if( 0 != sc_prm.s64_val0 )  { printf("[app] SM app configuration of Ethernet buffer memory size has failed: %d\n", sc_prm.s64_val0); exit(-1); }
    sc_prm.s64_val0 = APP_KEY_ETHQ_BUFF_RING_PTR;    /*  Key  */
    sc_prm.s64_val1 = EthBufPtr;    /*  Value  */
    if (ret = ioctl(fd_sm, SM_DRV_IOCTL_SET_CFG, (long)&sc_prm))    { printf("[app] SM_DRV_IOCTL_SET_CFG failed: %s\n", strerror(errno)); exit(ret); }
    if( 0 != sc_prm.s64_val0 )  { printf("[app] SM app configuration of Ethernet buffer pointer has failed: %d\n", sc_prm.s64_val0); exit(-1); }
#if 0
    sc_prm.s64_val0 = APP_KEY_STRM_VLAN_TAGS;    /*  Key  */
    sc_prm.s64_val1 = CFG_VLAN_TAGS;    /*  Value  */
    if (ret = ioctl(fd_sm, SM_DRV_IOCTL_SET_CFG, (long)&sc_prm))    { printf("[app] SM_DRV_IOCTL_SET_CFG failed: %s\n", strerror(errno)); exit(ret); }
    if( 0 != sc_prm.s64_val0 )  { printf("[app] SM app configuration of VLAN tags has failed: %d\n", sc_prm.s64_val0); exit(-1); }
#endif
    /*===== MJPEG decoder parameters =====*/
    ;

    /*  Install signal handler */
    sig_act.sa_sigaction = &sig_action;
    sig_act.sa_flags = SA_SIGINFO;
    if (sigaction(SIGPOLL, &sig_act, NULL))
        printf("[app] Can't install sig handler\n");

    /*  Register THIS process as an asynchronous events listener */
    mypid = getpid();
    if (ret = ioctl(fd_sm, SM_DRV_IOCTL_REG_SIG, (long)&mypid))
        printf("[app] SM_DRV_IOCTL_REG_SIG failed: %s\n", strerror(errno));

    /*  Enable events within the SM code */
    sc_prm.s64_val0 = 0x1; /* Non-zero value means: ENABLE, Zero value means: DISABLE */
    sc_prm.s64_val1 = 0x0; /* n/a */
    if (ret = ioctl(fd_sm, SM_DRV_IOCTL_ENABLE_EVENTS, (long)&sc_prm))
        printf("[app] SM_DRV_IOCTL_ENABLE_EVENTS (enable) failed: %s\n", strerror(errno));
    else
        printf("[app] SM_DRV_IOCTL_ENABLE_EVENTS (enable) returned: %lld\n", sc_prm.s64_val0);

    /*   Send "start" command to the SM app */
    if (ret = ioctl(fd_sm, SM_DRV_IOCTL_START, (long)&sc_prm))
        printf("[app] SM_DRV_IOCTL_START failed: %s\n", strerror(errno));
    else
        printf("[app] SM_DRV_IOCTL_START returned: %lld\n", sc_prm.s64_val0);

    /*  Let it run for 10 seconds */
    sleep(10);

    /*  Send "stop" command to the SM app */
    if (ret = ioctl(fd_sm, SM_DRV_IOCTL_STOP, (long)&sc_prm))
        printf("[app] SM_DRV_IOCTL_STOP failed: %s\n", strerror(errno));
    else
        printf("[app] SM_DRV_IOCTL_STOP returned: %lld\n", sc_prm.s64_val0);

    /*  Disable SM app events */
    sc_prm.s64_val0 = 0x0; /* Non-zero value means: ENABLE, Zero value means: DISABLE */
    sc_prm.s64_val1 = 0x0; /* n/a */
    if (ret = ioctl(fd_sm, SM_DRV_IOCTL_ENABLE_EVENTS, (long)&sc_prm))
        printf("[app] SM_DRV_IOCTL_ENABLE_EVENTS (disable) failed: %s\n", strerror(errno));
    else
        printf("[app] SM_DRV_IOCTL_ENABLE_EVENTS (disable) returned: %lld\n", sc_prm.s64_val0);

    /*  Unregister event listener */
    if (ret = ioctl(fd_sm, SM_DRV_IOCTL_UNREG_SIG, (long)&mypid))
        printf("[app] SM_DRV_IOCTL_UNREG_SIG failed: %s\n", strerror(errno));

    /*  TODO: read out and printf: State, ErrorMask and Benchmark results */
    ;

    close(fd_sm);

    sleep(1); /*  FIXME remove, temporary hack */

    /*  FIXME: dispose JPEG output buffer memory */
    /*  Dispose Eth buffer memory */
    ret = ioctl(fd_sram, IOCTL_SRAM_FREE, &sralloc_prm);
    if (ret)
        printf("[app] Unable to release memory: %d\n", ret);

    close(fd_sram);

    return EXIT_SUCCESS;
}
