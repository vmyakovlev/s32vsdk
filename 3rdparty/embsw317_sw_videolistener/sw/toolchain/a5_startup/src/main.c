#if (TRUE == CFG_LINK_USER_CODE)
void entry_c(void);
#endif /* CFG_LINK_USER_CODE */

/*  Das main */
int main(int argc, char *argv)
{
#if (TRUE == CFG_LINK_USER_CODE)
    entry_c();
#endif /* CFG_LINK_USER_CODE */
	while (1);
	return 0;
}
