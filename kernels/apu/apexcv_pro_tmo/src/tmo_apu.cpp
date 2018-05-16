/**************************************************************************************************
 * 
 * NXP Confidential Proprietary
 *
 * Copyright (c) 2015, Digital Media Professionals. All rights reserved.
 * Copyright 2017 NXP 
 * All Rights Reserved
 *
 *****************************************************************************
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 **************************************************************************************************/


#ifdef ACF_KERNEL_IMPLEMENTATION

static const int08u gsExpTableData[] = {
   128, 128, 128, 129, 129, 129, 130, 130,
   130, 131, 131, 131, 132, 132, 132, 133,
   133, 134, 134, 134, 135, 135, 135, 136,
   136, 136, 137, 137, 138, 138, 138, 139,
   139, 139, 140, 140, 141, 141, 141, 142,
   142, 143, 143, 143, 144, 144, 144, 145,
   145, 146, 146, 146, 147, 147, 148, 148,
   148, 149, 149, 150, 150, 150, 151, 151,
   152, 152, 153, 153, 153, 154, 154, 155,
   155, 155, 156, 156, 157, 157, 158, 158,
   158, 159, 159, 160, 160, 161, 161, 161,
   162, 162, 163, 163, 164, 164, 165, 165,
   165, 166, 166, 167, 167, 168, 168, 169,
   169, 170, 170, 171, 171, 171, 172, 172,
   173, 173, 174, 174, 175, 175, 176, 176,
   177, 177, 178, 178, 179, 179, 180, 180,
   181, 181, 182, 182, 182, 183, 183, 184,
   184, 185, 185, 186, 186, 187, 188, 188,
   189, 189, 190, 190, 191, 191, 192, 192,
   193, 193, 194, 194, 195, 195, 196, 196,
   197, 197, 198, 199, 199, 200, 200, 201,
   201, 202, 202, 203, 203, 204, 205, 205,
   206, 206, 207, 207, 208, 208, 209, 210,
   210, 211, 211, 212, 212, 213, 214, 214,
   215, 215, 216, 217, 217, 218, 218, 219,
   219, 220, 221, 221, 222, 222, 223, 224,
   224, 225, 226, 226, 227, 227, 228, 229,
   229, 230, 230, 231, 232, 232, 233, 234,
   234, 235, 236, 236, 237, 237, 238, 239,
   239, 240, 241, 241, 242, 243, 243, 244,
   245, 245, 246, 247, 247, 248, 249, 249,
   250, 251, 251, 252, 253, 253, 254, 255
};

static const int16u gsLogTableData[] = {
      0,  149,  338,  462,  555,  629,  691,  744,
    790,  831,  868,  902,  932,  961,  987, 1012,
   1035, 1057, 1077, 1097, 1115, 1133, 1149, 1165,
   1181, 1196, 1210, 1224, 1237, 1249, 1262, 1274,
   1285, 1296, 1307, 1318, 1328, 1338, 1348, 1357,
   1367, 1376, 1384, 1393, 1401, 1409, 1418, 1425,
   1433, 1441, 1448, 1455, 1462, 1469, 1476, 1483,
   1489, 1496, 1502, 1509, 1515, 1521, 1527, 1533,
   1538, 1544, 1550, 1555, 1561, 1566, 1571, 1576,
   1582, 1587, 1592, 1597, 1601, 1606, 1611, 1616,
   1620, 1625, 1629, 1634, 1638, 1642, 1647, 1651,
   1655, 1659, 1663, 1668, 1672, 1676, 1679, 1683,
   1687, 1691, 1695, 1698, 1702, 1706, 1709, 1713,
   1717, 1720, 1724, 1727, 1730, 1734, 1737, 1741,
   1744, 1747, 1750, 1754, 1757, 1760, 1763, 1766,
   1769, 1772, 1775, 1778, 1781, 1784, 1787, 1790,
   1793, 1796, 1799, 1801, 1804, 1807, 1810, 1813,
   1815, 1818, 1821, 1823, 1826, 1829, 1831, 1834,
   1836, 1839, 1841, 1844, 1846, 1849, 1851, 1854,
   1856, 1859, 1861, 1863, 1866, 1868, 1870, 1873,
   1875, 1877, 1880, 1882, 1884, 1886, 1889, 1891,
   1893, 1895, 1897, 1900, 1902, 1904, 1906, 1908,
   1910, 1912, 1914, 1916, 1918, 1920, 1923, 1925,
   1927, 1929, 1931, 1932, 1934, 1936, 1938, 1940,
   1942, 1944, 1946, 1948, 1950, 1952, 1954, 1955,
   1957, 1959, 1961, 1963, 1965, 1966, 1968, 1970,
   1972, 1973, 1975, 1977, 1979, 1980, 1982, 1984,
   1986, 1987, 1989, 1991, 1992, 1994, 1996, 1997,
   1999, 2001, 2002, 2004, 2006, 2007, 2009, 2010,
   2012, 2014, 2015, 2017, 2018, 2020, 2021, 2023,
   2024, 2026, 2027, 2029, 2031, 2032, 2034, 2035,
   2037, 2038, 2039, 2041, 2042, 2044, 2045, 2047
};

static const int16u gsDivTableData[] = {
      4,  0, 5461, 12, 6553, 13, 4681, 13, 7281, 14, 5957, 14, 5041, 14, 4369, 14,
   3855, 14, 3449, 14, 6241, 15, 2849, 14, 2621, 14, 2427, 14, 4519, 15, 1057, 13,
   7943, 16, 7489, 16, 1771, 14, 6721, 16, 6393, 16,  381, 12, 5825, 16, 5577, 16,
   5349, 16, 1285, 14, 2473, 15, 2383, 15, 4599, 16, 4443, 16, 4297, 16, 4161, 16,
   8065, 17, 7825, 17, 3799, 16,  923, 14, 3591, 16, 3495, 16,  851, 14, 1659, 15,
    809, 14, 1579, 15,  771, 14, 3013, 16, 2945, 16, 5761, 17, 5637, 17, 2759, 16,
   5405, 17, 5295, 17, 2595, 16, 2545, 16, 4993, 17, 4899, 17, 4809, 17, 4723, 17,
   4639, 17, 4559, 17, 4481, 17, 4405, 17, 1083, 15, 2131, 16, 2097, 16,  129, 12,
    127, 12, 2001, 16, 1971, 16, 7767, 18, 7653, 18, 7543, 18, 1859, 16, 1833, 16,
   7231, 18, 7133, 18, 7037, 18,  217, 13, 6853, 18, 6765, 18, 3339, 17, 3297, 17,
    407, 14,  201, 13, 6355, 18, 3139, 17, 1551, 16, 1533, 16, 6061, 18, 5991, 18,
   1481, 16, 5857, 18, 5793, 18, 5729, 18, 5667, 18, 5607, 18, 1387, 16, 5489, 18,
   5433, 18, 5377, 18, 2661, 17, 5269, 18,  163, 13, 5165, 18, 5115, 18, 5065, 18,
   5017, 18, 4969, 18, 2461, 17, 4877, 18,  151, 13, 1197, 16,  593, 15, 2351, 17,
   1165, 16, 4619, 18, 2289, 17, 4539, 18, 1125, 16, 2231, 17,  553, 15, 4387, 18,
   2175, 17, 4315, 18, 4279, 18, 4245, 18, 4211, 18, 4177, 18,  259, 14,  257, 14,
    255, 14, 8097, 19, 8035, 19, 7973, 19, 7913, 19, 3927, 18, 1949, 17, 3869, 18,
   7681, 19, 3813, 18, 3785, 18, 1879, 17, 7463, 19, 3705, 18, 3679, 18, 7307, 19,
    907, 16, 3603, 18, 7157, 19, 1777, 17, 7061, 19, 7013, 19, 6967, 19, 6921, 19,
   6875, 19, 6831, 19, 3393, 18, 6743, 19, 1675, 17, 6657, 19, 6615, 19, 3287, 18,
   6533, 19, 1623, 17, 1613, 17, 6413, 19, 3187, 18, 6335, 19, 6297, 19, 1565, 17,
   6223, 19, 3093, 18, 3075, 18, 3057, 18, 3039, 18, 6043, 19, 6009, 19, 2987, 18,
   1485, 17, 5907, 19, 2937, 18, 5841, 19, 5809, 19, 5777, 19, 5745, 19, 2857, 18,
   5683, 19, 1413, 17, 2811, 18,  699, 16, 2781, 18, 5533, 19,   43, 12, 5475, 19,
   5447, 19, 2709, 18, 5391, 19, 5363, 19,  667, 16, 5309, 19, 2641, 18,  657, 16,
   5229, 19, 5203, 19, 2589, 18,  161, 14, 5127, 19, 2551, 18, 5077, 19, 5053, 19,
   5029, 19, 5005, 19, 4981, 19, 4957, 19, 2467, 18, 4911, 19,  611, 16, 4865, 19,
   4843, 19, 4821, 19, 2399, 18, 4777, 19, 4755, 19, 4733, 19,  589, 16, 4691, 19,
   2335, 18, 2325, 18, 4629, 19, 4609, 19, 1147, 17,  571, 16, 4549, 19, 4529, 19,
   2255, 18, 2245, 18, 4471, 19, 1113, 17, 4433, 19, 4415, 19, 1099, 17, 2189, 18,
   4359, 19, 4341, 19, 1081, 17, 2153, 18,   67, 13, 4271, 19, 4253, 19, 1059, 17,
   4219, 19, 2101, 18, 4185, 19, 4169, 19,  519, 16,  517, 16,  515, 16,  513, 16
};

/////// end LUT data ///////

static const int16u LOG_TABLE_SIZE = 256;
static const int16u EXP_TABLE_SIZE = 256;
static const int16u DIV_TABLE_SIZE = 512;
static const int16u RGBE_DATA_SIZE = 4;
static const int16u RGB_DATA_SIZE  = 3;
static const int32u LOG_ROUND_MASK = 0xff;
static const int16u LOG_SHIFT      = 8;

static inline void _calc_world_luminance(vec08u *ie, vec08u *im, vec16s *logTable, vec08u *we, vec08u *wm);
static inline void _calc_scaled_luminance(vec08u we, vec08u wm, int08u geoMe, int16u *divValue, vec16s *logTable, int08u sKey, vec08u *se, vec08u *sm);
static void _calc_display_luminance(vec08u se, vec08u sm, vec16s *logTable, vec08u *de, vec08u *dm);
static inline vec08u _calc_final_rgb(vec08u de, vec08u dm, vec08u we, vec08u wm, vec08u ie, vec08u im, vec16u *divTable);
static inline int16u _ceil_n_shift(int32s value, int32u mask, int08u shift);
static inline vec16u _ceil_n_shift(vec32u value, int32u mask, int08u shift);
static inline vec16u _ceil_n_shift(vec16u value, int32u mask, int08u shift);
static inline vec16s _ceil_n_shift(vec16s value, int32u mask, int08u shift);
static inline vec32u _round_n_shift(vec32u value, int32u mask, int08u shift);
static inline int32s _clamp(int32s value, int32s left, int32s right);
static inline vec16u _vclamp(vec16u value, vec16u left, vec16u right);
static inline vec16s _vclamp(vec16s value, vec16s left, vec16s right);
static inline vec32u _vclamp(vec32u value, vec32u left, vec32u right);
static inline vec08u _vmax(vec08u a, vec08u b, vec08u c);
static inline vec32s _find_log2(vec32u value);



/*!
   Calculate world luminance for RGBE input image.
   (unsigned 8-bit -> unsigned 8-bit)

   \param lpvIn         - [Input]   Pointer to the input image data.
   \param lStrideIn     - [Input]   Stride of the input image data.
   \param lpvOutIe      - [Output]  Pointer to the output intermediate exponent data.
   \param lpvOutIm      - [Output]  Pointer to the output intermediate mantissa data.
   \param lStrideInt    - [Input]   Stride of the output intermediate data.
   \param lpvOutWe      - [Output]  Pointer to the output world luminance exponent data.
   \param lpvOutWm      - [Output]  Pointer to the output world luminance mantissa data.
   \param lStrideWor    - [Input]   Stride of the output world luminance data.
   \param logTable      - [Input]   Pointer to the input log LUT data.
   \param lpvSumWe      - [Output]  Pointer to the output geometric sum exponent data.
   \param lpvSumWm      - [Output]  Pointer to the output geometric sum mantissa data.
   \param lpvSumCnt     - [Output]  Pointer to the output geometric count data.
   \param lChunkWidth   - [Input]   Processing chunk width.
   \parma lChunkHeight  - [Input]   Processing chunk height.
   */
void apu_tmo_calc_world_lum_rgbe(
   vec08u *apvIn,                   // RGBE Data, 32 bits per pixel
   int16u aStrideIn,
   vec08u *apvOutIe,                // 3x Replicated E from RGBE data, 24 bits per pixel
   vec08u *apvOutIm,                // RGB from RGBE data, 24 bits per pixel
   int16u aStrideInt,
   vec08u *apvOutWe,                // 8 bits per pixel
   vec08u *apvOutWm,                // 8 bits per pixel
   int16u aStrideWor,
   vec16s *apLogTable,                // 16 bytes x 256 entries
   vec32s *apvSumWe,                // Single entry
   vec32s *apvSumWm,                // Single entry
   vec32u *apvSumCnt,               // Single entry
   int16u aChunkWidth,
   int16u aChunkHeight,
   bool bFirstTile
   )
{
   vec32s localSumWe = 0;
   vec32s localSumWm = 0;
   vec32u localCnt = 0;

   if(bFirstTile == true)
   {
      for (int16u i = 0; i < LOG_TABLE_SIZE; i++)
      {
         apLogTable[i] = (vec16s)gsLogTableData[i];
      }
   }

   const vec08u* restrict lpvIn    = apvIn;
         vec08u* restrict lpvOutIe = apvOutIe;
         vec08u* restrict lpvOutIm = apvOutIm;

         vec08u* restrict lpvOutWe = apvOutWe;
         vec08u* restrict lpvOutWm = apvOutWm;

   for (int16u h = 0; h < aChunkHeight; h++)
   {
      for (int16u w = 0; w < aChunkWidth; w++)
      {
         // convert RGBE format to intermediate format
         for (int16u k = 0; k < RGB_DATA_SIZE; k++)
         {
            lpvOutIe[w*RGB_DATA_SIZE + k] = lpvIn[w*RGBE_DATA_SIZE + 3];
            lpvOutIm[w*RGB_DATA_SIZE + k] = lpvIn[w*RGBE_DATA_SIZE + k];
         }

         // calculate world luminance
         _calc_world_luminance(&lpvOutIe[w*RGB_DATA_SIZE],
                               &lpvOutIm[w*RGB_DATA_SIZE],
                               apLogTable,
                               &apvOutWe[w],
                               &apvOutWm[w]);

         vif(apvOutWe[w] != 0)// only non-zero Lwe are used
         {
            // Σ(Lwe - 136)
            // Σlog2(Lwm + 0.5)
            localSumWe += (vec32s)apvOutWe[w] - 136;
            localSumWm += (vec32s)vload(apLogTable, apvOutWm[w]);
            localCnt++;
         }
         vendif
      }
      lpvIn    += aStrideIn;
      lpvOutIe += aStrideInt;
      lpvOutIm += aStrideInt;
      apvOutWe += aStrideWor;
      apvOutWm += aStrideWor;
   }
   apvSumWe[0] += localSumWe;
   apvSumWm[0] += localSumWm;
   apvSumCnt[0] += localCnt;
}
/*!
   Calculate world luminance for OpenEXR input image.
   (unsigned 16-bit -> unsigned 8-bit)

   \param lpvIn         - [Input]   Pointer to the input image data.
   \param lStrideIn     - [Input]   Stride of the input image data.
   \param lpvOutIe      - [Output]  Pointer to the output intermediate exponent data.
   \param lpvOutIm      - [Output]  Pointer to the output intermediate mantissa data.
   \param lStrideInt    - [Input]   Stride of the output intermediate data.
   \param lpvOutWe      - [Output]  Pointer to the output world luminance exponent data.
   \param lpvOutWm      - [Output]  Pointer to the output world luminance mantissa data.
   \param lStrideWor    - [Input]   Stride of the output world luminance data.
   \param logTable      - [Input]   Pointer to the input log LUT data.
   \param lpvSumWe      - [Output]  Pointer to the output geometric sum exponent data.
   \param lpvSumWm      - [Output]  Pointer to the output geometric sum mantissa data.
   \param lpvSumCnt     - [Output]  Pointer to the output geometric count data.
   \param lChunkWidth   - [Input]   Processing chunk width.
   \parma lChunkHeight  - [Input]   Processing chunk height.
   */
void apu_tmo_calc_world_lum_openexr(
   vec16u *lpvIn,
   int16u lStrideIn,
   vec08u *lpvOutIe,
   vec08u *lpvOutIm,
   int16u lStrideInt,
   vec08u *lpvOutWe,
   vec08u *lpvOutWm,
   int16u lStrideWor,
   vec16s *logTable,
   vec32s *lpvSumWe,
   vec32s *lpvSumWm,
   vec32u *lpvSumCnt,
   int16u lChunkWidth,
   int16u lChunkHeight,
   bool bFirstTile
   )
{

   vec32s localSumWe = 0;
   vec32s localSumWm = 0;
   vec32u localCnt = 0;

   if(bFirstTile == true)
   {
      for (int16u i = 0; i < LOG_TABLE_SIZE; i++)
      {
         logTable[i] = (vec16s)gsLogTableData[i];
      }
   }

   for (int16u h = 0; h < lChunkHeight; h++)
   {
      for (int16u w = 0; w < lChunkWidth; w++)
      {
         // convert OpenEXR format to intermediate format
         for (int16u k = 0; k < RGB_DATA_SIZE; k++)
         {
            vec16u srcBit = lpvIn[w*RGB_DATA_SIZE + k];
            lpvOutIe[w*RGB_DATA_SIZE + k] = (vec08u)((srcBit >> 10) & 0x1f) + 128 - 16;
            lpvOutIm[w*RGB_DATA_SIZE + k] = (vec08u)((srcBit & 0x3ff) >> 3) | 0x80;
         }

         // calculate world luminance
         _calc_world_luminance(&lpvOutIe[w*RGB_DATA_SIZE],
                               &lpvOutIm[w*RGB_DATA_SIZE],
                               logTable,
                               &lpvOutWe[w],
                               &lpvOutWm[w]);

         vif(lpvOutWe[w] != 0)// only non-zero Lwe are used
         {
            // Σ(Lwe - 136)
            // Σlog2(Lwm + 0.5)
            localSumWe += (vec32s)lpvOutWe[w] - 136;
            localSumWm += (vec32s)vload(logTable, lpvOutWm[w]);
            localCnt++;
         }
         vendif
      }
      lpvIn += lStrideIn;
      lpvOutIe += lStrideInt;
      lpvOutIm += lStrideInt;
      lpvOutWe += lStrideWor;
      lpvOutWm += lStrideWor;
   }
   lpvSumWe[0] += localSumWe;
   lpvSumWm[0] += localSumWm;
   lpvSumCnt[0] += localCnt;
}

/*!
   Calculate geometric mean.
   (unsigned 32-bit -> unsigned 8-bit)

   \param lpvInSumWe    - [Input]   Pointer to the input geometric sum exponent data.
   \param lpvInSumWm    - [Input]   Pointer to the input geometric sum mantissa data.
   \param lpvInCnt      - [Input]   Pointer to the input geometric count data.
   \param expTable      - [Input]   Pointer to the input exponent LUT data.
   \param lpvOutGeoMe   - [Output]  Pointer to the output geometric mean exponent data.
   \param lpvOutGeoMm   - [Output]  Pointer to the output geometric mean mantissa data.
   */
void apu_tmo_calc_geo_mean(
   vec32s lpvInSumWe,
   vec32s lpvInSumWm,
   vec32u lpvInCnt,
   int16u start_cu,
   int16u end_cu,
   int08u *expTable,
   int08u *lpvOutGeoMe,
   int08u *lpvOutGeoMm
   )
{
   int32s globalSumWe = 0;
   int32s globalSumWm = 0;
   int32u globalCnt = 0;

   for (int16u i = 0; i < EXP_TABLE_SIZE; i++)
   {
      expTable[i] = gsExpTableData[i];
   }

   // calculation of geometric mean
   // SLwe = 1/N*Σ(Lwe - 136)
   // SLwm = 1/N*Σlog2(Lwm + 0.5)
   // Gwe = SLwm + SLwe + 128
   // Gwm = 2^(SLwm+SLwe-Gwe+136)
   for (int16u cu = start_cu; cu < end_cu; cu++)
   {
      globalSumWe += vget(lpvInSumWe, cu);
      globalSumWm += vget(lpvInSumWm, cu);
      globalCnt += vget(lpvInCnt, cu);
   }
   int32s SLwe = (int32s)((float)(globalSumWe)* 256 / (float)globalCnt);
   int32s SLwm = globalSumWm / globalCnt;

   lpvOutGeoMe[0] = (int08u)_ceil_n_shift(SLwm + SLwe + (128 * 256), LOG_ROUND_MASK, LOG_SHIFT);

   int08u exp2Index = _clamp(SLwm + SLwe + ((-lpvOutGeoMe[0] + 136 - 7) << LOG_SHIFT), 0, 255);
   lpvOutGeoMm[0] = expTable[exp2Index];
}

/*!
   LDR transformation.
   (unsigned 8-bit -> unsigned 8-bit)

   \param lpvInGeoMe    - [Input]   Pointer to the input geometric mean exponent data.
   \param lpvInIe       - [Input]   Pointer to the input intermediate exponent data.
   \param lpvInIm       - [Input]   Pointer to the input intermediate mantissa data.
   \param lStrideIn     - [Input]   Stride of the input intermediate data.
   \param lpvInWe       - [Input]   Pointer to the input world luminance exponent data.
   \param lpvInWm       - [Input]   Pointer to the input world luminance mantissa data.
   \param lStrideInW    - [Input]   Stride of the input world luminance data.
   \param divValue      - [Input]   Pointer to the input division value.
   \param logTable      - [Input]   Pointer to the input log LUT data.
   \param divTable      - [Input]   Pointer to the input division LUT data.
   \param keyValue      - [Input]   Input key value data.
   \param lpvOut        - [Output]  Pointer to the output ldr image data.
   \param lStrideOut    - [Input]   Stride of the output ldr image data.
   \param lChunkWidth   - [Input]   Processing chunk width.
   \param lChunkHeight  - [Input]   Processing chunk height.
   */
void apu_tmo_ldr_transform(
   int08u  apvInGeoMe,
   vec08u* apvInIe,
   vec08u* apvInIm,
   int16u  aStrideIn,
   vec08u* apvInWe,
   vec08u* apvInWm,
   int16u  aStrideInW,
   int16u* apDivValue,
   vec16s* apvLogTable,
   vec16u* apvDivTable,
   int08u  aKeyVal,
   vec08u* apvOut,
   int16u  aStrideOut,
   int16u  aChunkWidth,
   int16u  aChunkHeight,
   bool bFirstTile
   )
{
   vec08u se, sm, de, dm;

   if(bFirstTile == true)
   {
      for (int16u i = 0; i < LOG_TABLE_SIZE; i++) chess_loop_range(1,) // 256
      {
         apvLogTable[i] = (vec16s)gsLogTableData[i];
      }

      for (int16u i = 0; i < DIV_TABLE_SIZE; i++) chess_loop_range(1,) // 512
      {
         apvDivTable[i] = (vec16u)gsDivTableData[i];
      }
   }

   for (int16u h = 0; h < aChunkHeight; h++) chess_loop_range(1,)
   {
      for (int16u w = 0; w < aChunkWidth; w++) chess_loop_range(1,)
      {
         // calculate scaled luminance
         _calc_scaled_luminance(apvInWe[w],
                                apvInWm[w],
                                apvInGeoMe,
                                apDivValue,
                                apvLogTable,
                                aKeyVal,
                                &se,
                                &sm);

         // calculate display luminance
         _calc_display_luminance(se,
                                 sm,
                                 apvLogTable,
                                 &de,
                                 &dm);

         for (int16u c = 0; c < RGB_DATA_SIZE; c++) chess_loop_range(1,)
         {
            apvOut[w*RGB_DATA_SIZE + c] = _calc_final_rgb(de,
                                                          dm,
                                                          apvInWe[w],
                                                          apvInWm[w],
                                                          apvInIe[w*RGB_DATA_SIZE + c],
                                                          apvInIm[w*RGB_DATA_SIZE + c],
                                                          apvDivTable);
         }
      }
      apvInIe += aStrideIn;
      apvInIm += aStrideIn;
      apvInWe += aStrideInW;
      apvInWm += aStrideInW;
      apvOut  += aStrideOut;
   }
}

static inline void _calc_world_luminance(vec08u* ie,
                                         vec08u* im,
                                         vec16s* logTable,
                                         vec08u* we,
                                         vec08u* wm)
{
   // calculation of world luminance
   // ML = 0.27*(Rm+0.5)*2^Re + 0.67*(Gm+0.5)*2^Ge + 0.06*(Bm+0.5)*2^Be
   // Lwe = log2(ML) - 8
   // Lwm = ML*2^(-Lwe)

   static const int16s s_k0 = 4424;//0.27*(1<<14)
   static const int16s s_k1 = 10977;//0.67*(1<<14)
   static const int16s s_k2 = 983;//0.06*(1<<14)

   // ML = 0.27*(Rm+0.5)*2^Re + 0.67*(Gm+0.5)*2^Ge + 0.06*(Bm+0.5)*2^Be
   vec08u maxE = _vmax(ie[0],
                       ie[1],
                       ie[2]);

   vec32s ML = ((s_k0 * (vec32s)(((vec16s)im[0] << 1) + 1)) >> (vec32s)(maxE - ie[0])) +
               ((s_k1 * (vec32s)(((vec16s)im[1] << 1) + 1)) >> (vec32s)(maxE - ie[1])) +
               ((s_k2 * (vec32s)(((vec16s)im[2] << 1) + 1)) >> (vec32s)(maxE - ie[2]));
   ML >>= 15;

   vec16s logML = vload(logTable, (vec08u)_vclamp((vec16s)ML, 0, 255));

   // Lwe = log2(ML) - 8
   vec08u te = (vec08u)_ceil_n_shift(logML, LOG_ROUND_MASK, LOG_SHIFT) + maxE - 8;

   we[0] = (vec08u)vselect(0, te, maxE == 0);

   // Lwm = ML*2^(-Lwe)
   vec08u tm = (vec08u)_vclamp((vec16s)(ML << (vec32s)(maxE - we[0])), 0, 255);

   wm[0] = (vec08u)vselect(0, tm, maxE == 0);
}

static inline void _calc_scaled_luminance(vec08u we,
                                          vec08u wm,
                                          int08u geoMe,
                                          int16u *divValue,
                                          vec16s *logTable,
                                          int08u sKey,
                                          vec08u *se,
                                          vec08u *sm)
{
   // calculation of scaled luminance
   // AL = key*(Lwm+0.5)/(Gwm+0.5)
   // Le = log2(AL)+Lwe-Gwe+128
   // Lm = AL*2^(136+Lwe-Le-Gwe)

   // AL = (key*(Lwm+0.5)/(Gwm+0.5))<<DIV_SHIFT, where DIV_SHIFT=2
   vec16u al = (vec16u)(((sKey*(((vec32u)wm << 1) + 1)) * divValue[0]) >> divValue[1]);
   vec08u alShifted = (vec08u)_vclamp(al >> 2, 0, 255);

   // Le = log2(AL)+Lwe-Gwe+128-KEY_VALUE_SHIFT, +1 reduce the error at the end
   vec32s le = (vec32s)vload(logTable, alShifted) + ((vec32s)(-8 + (vec16s)we - (vec16s)geoMe + 128 + 1) << LOG_SHIFT);
   vec16s leShifted = (vec16s)_ceil_n_shift((vec32u)le, LOG_ROUND_MASK, LOG_SHIFT);
   se[0] = (vec08u)_vclamp(leShifted, 0, 255);

   // Lm = AL*2^(136+Lwe-Le-Gwe-DIV_SHIFT-KEY_VALUE_SHIFT)
   vec16s shift = 136 + (vec16s)we - (vec16s)se[0] - (vec16s)geoMe - 10;
   vec16u lm = vselect(al >> vabs(shift), al << shift, shift < 0);
   lm = _vclamp(lm, 0, 255);
   sm[0] = (vec08u)vselect(se[0], (vec08u)lm, (leShifted < 0) || (leShifted>255));
}



static void _calc_display_luminance(vec08u se, vec08u sm, vec16s *logTable, vec08u *de, vec08u *dm)
{
   // calculation of display luminance
   // FL = (Lm + 0.5) / (Lm + 0.5 + 2^(136 - Le))
   // Lde = log2(FL) + 128
   // Ldm = FL*2^(136 - Lde)   
   vec32s tDe;
   vec16s tse = (136 - (vec16s)se);
   vec32u sm2 = ((vec32u)sm << 1) + 1;

   // calculation falls into one of the following 3 cases:
   // Case 1: (136-Le) > 15
   // FL can be approximated as: FL = (Lm + 0.5) / (2^(136-Le)), because 2^(136-Le) is very large
   // Lde = log2(Lm+0.5) - 136 + Le + 128
   // Ldm = (Lm + 0.5)*2^(Le - Lde)
   //
   // Case 2: (136-Le) < -8
   // FL can be approximated as 1, becasue 2^(136-Le) is very small and can be ignored
   // Lde = 128
   // Ldm = 255
   //
   // Case 3: -8 <= (136-Le) <= 15
   // in this case, Lde and Ldm can be calculated with fixed-point arithmetic
   vif(tse > 15 || tse < -8)//Case 1 || Case 2
   {
      // Lde = log2(Lm) - 136 + Le + 128
      // Ldm = (Lm+0.5)*2^(Le-Lde)
      tDe = (vec32s)vload(logTable, sm) + ((-136 + (vec32s)se + 128) << 8);

      // Output calculated Lde for Case 1, 128 for Case 2
      de[0] = (vec08u)vselect((vec16s)_vclamp(_ceil_n_shift((vec32u)tDe, 0xff, 8), 0, 255), (vec16s)se, tse > 15);

      // Output calculated Ldm for Case 1, 255 for Case 2
      dm[0] = (vec08u)vselect(_vclamp((vec16s)((sm2 << (vec32u)(se - de[0])) >> 1), 0, 255), 255, tse > 15);
   }
   velse//Case 3
   {
      // Lde = log2(Lm+0.5) - log2(Lm + 0.5 + 2^(136-Le))
      // if (136-Le)<0, Lde = log2(Lm+0.5) - (log2( (Lm+0.5)<<abs(136-Le+1) + (1<<1) ) - abs(136-Le+1)) + 128
      // if (136-Le)>=0, Lde = log2(Lm+0.5) - (log2( ((Lm+0.5)<<1) + (1<<(136-Le+1) ) - 1) + 128
      vec08u a = (vec08u)vselect(vabs(tse) + 1, 1, tse < 0);
      vec32u base = _vclamp(((vec32u)sm2 << (vec32u)(a - 1)) + ((vec32u)1 << (vec32u)(tse + (vec16s)a)), 0, 0xffff);
      vec32s logResl = _find_log2(base * base);//The result will be more precise to calculate base*base.
      tDe = (((vec32s)vload(logTable, sm) << 1) - ((logResl - (((vec32s)a + 128) << 1)) << LOG_SHIFT)) >> 1;//log2(base*base) = 2 * log2(base)
      vec16u localDe = _ceil_n_shift((vec32u)tDe, LOG_ROUND_MASK, LOG_SHIFT);

      // Ldm = (Lm+0.5) * 2^(136-Lde) / (Lm + 0.5 + 2^(136-Le))
      vec32u div = vselect((vec32u)0, (vec32u)1 << (vec32u)(tse + 1), tse < -1);
      vec32u tDm = ((vec32u)sm2 << (vec32u)(136 - localDe)) / ((vec32u)sm2 + div);

      de[0] = (vec08u)_vclamp(localDe, 0, 255);
      dm[0] = (vec08u)vselect(de[0], (vec08u)_vclamp(tDm, 0, 255), localDe < 0 || localDe > 255);
   }
      vendif
}

static inline vec08u _calc_final_rgb(vec08u de, vec08u dm, vec08u we, vec08u wm, vec08u ie, vec08u im, vec16u *divTable)
{
   // calculation of final RGB value
   // RL = (Ldm+0.5)*(Cm+0.5)/(Lwm+0.5)
   // Ci = round(255*RL*2^(Ce+Lde-Lwe-136))

   // RL = (Ldm+0.5)*(Cm+0.5)/(Lwm+0.5), division replaced by mult+shift
   // LUT is created to produce the following division result: when B and C are given, LUT returns A, where A = (B<<1)/C
   // Therefore, the following calculation is actually computing:
   // RL' = ((((Ldm+0.5)<<1)*((Cm+0.5)<<1))<<1)/(Lwm+0.5), thus RL' = RL<<3   
   vec16u mul = vload(divTable, (vec16s)wm * 2);
   vec16u shift = vload(divTable, (vec16s)wm * 2 + 1);
   vec32u rl = (((((vec32u)dm << 1) + 1) * (((vec32u)im << 1) + 1))*(vec32u)mul) >> (vec32u)shift;

   // expo = Ce+Lde-Lwe-136
   vec16s expo = (vec16s)ie + (vec16s)de - (vec16s)we - 136;
   expo = _vclamp(expo, -31, 31);

   // Ci = (round((255<<5)*RL'*2^expo)>>8)
   vec32u color = vselect((vec32u)(rl * 8160) >> (vec32u)vabs(expo), (rl * 8160 << (vec32u)expo), expo < 0);
   color = _round_n_shift(color, 0xff, 8);

   return (vec08u)_vclamp((vec16s)color, 0, 255);
}

static inline int16u _ceil_n_shift(int32s value, int32u mask, int08u shift)
{
   return (int16u)((value & mask) ? ((value >> shift) + 1) : (value >> shift));
}

static inline vec16u _ceil_n_shift(vec32u value, int32u mask, int08u shift)
{
   return vselect((vec16u)((value >> shift) + 1), (vec16u)(value >> shift), (value & mask) != 0);
}

static inline vec16s _ceil_n_shift(vec32s value, int32u mask, int08u shift)
{
   vec16s tmp = (vec16s)(value >> shift);
   vec16s bit = (vec16s)vselect((vec08u)1, (vec08u)0, (value & mask) != 0);
   return tmp + bit;
   //   return vselect((vec16s)((value>>shift)+1), (vec16s)(value>>shift), (value & mask)!=0);
}

static inline vec16u _ceil_n_shift(vec16u value, int32u mask, int08u shift)
{
   return vselect((value >> shift) + 1, value >> shift, (value & mask) != 0);
}

static inline vec16s _ceil_n_shift(vec16s value, int32u mask, int08u shift)
{
   return vselect((value >> shift) + 1, value >> shift, (value & mask) != 0);
}

static inline vec32u _round_n_shift(vec32u value, int32u mask, int08u shift)
{
   return vselect(value >> shift, (value >> shift) + 1, ((value & mask) < (mask >> 1)));
}

static inline int32s _clamp(int32s value, int32s left, int32s right)
{
   return ((value < left) ? left : ((value > right) ? right : value));
}

static inline vec16u _vclamp(vec16u value, vec16u left, vec16u right)
{
   vec16u t = vselect(value, left, left < value);
   return vselect(t, right, t < right);
}

static inline vec16s _vclamp(vec16s value, vec16s left, vec16s right)
{
   vec16s t = vselect(value, left, left < value);
   return vselect(t, right, t < right);
}

static inline vec32u _vclamp(vec32u value, vec32u left, vec32u right)
{
   vec32u t = vselect(value, left, left < value);
   return vselect(t, right, t < right);
}

static inline vec08u _vmax(vec08u a, vec08u b, vec08u c)
{
   vec08u max = (vec08u)vselect(a, b, a > b);
   return (vec08u)vselect(max, c, max > c);
}

static inline vec32s _find_log2(vec32u value)
{
   const vec16u cvCountZeroHigh = vclz(vec16u(value>>16));
   const vec16u cvCountZeroLow  = vclz(vec16u(value&0xFFFF));

   return (vec32s)vselect(15-cvCountZeroLow,
                          31-cvCountZeroHigh,
                          cvCountZeroHigh == 16);
}

#endif // ACF_KERNEL_IMPLEMENTATION
