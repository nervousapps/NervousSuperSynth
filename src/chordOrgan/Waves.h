#ifndef Waves_h
#define Waves_h

// Custom Waveforms. 16 bit.
const int16_t wave0[257] = {
    -2030,-6562,-10879,-14234,-17043,-19141,-20763,-21892,-22661,-23100,
    -23251,-23143,-22740,-22045,-20957,-19461,-17417,-14804,-11491,-7497,
    -2761,2595,8476,14578,20586,25959,30160,32490,32349,29181,
    22835,13595,2801,-8438,-18631,-25361,-27200,-23449,-14635,-2111,
    11686,23947,31502,31973,24161,9848,-6643,-20748,-27176,-23616,
    -11383,5676,21952,31726,30679,18160,-732,-18428,-27135,-22761,
    -7843,11628,27412,32681,23808,4818,-15210,-26671,-23629,-8467,
    11819,27968,32480,22083,2086,-17618,-27181,-21919,-5438,14671,
    29385,32008,20491,726,-18081,-27163,-22591,-7608,11439,26845,
    32748,26286,10256,-8394,-22800,-27250,-21051,-7231,9411,23818,
    31849,31285,22228,7862,-7362,-20001,-26612,-26321,-20008,-9614,
    2609,14457,24145,30428,32698,30971,25781,18126,9251,514,
    -7708,-14866,-20401,-24230,-26446,-27299,-27083,-26095,-24615,-22885,
    -21100,-19417,-17953,-16797,-16003,-15608,-15629,-16065,-16899,-18094,
    -19588,-21289,-23077,-24784,-26213,-27099,-27173,-26108,-23627,-19470,
    -13587,-6105,2307,11224,20058,27331,31835,32603,29232,21817,
    11217,-1151,-13211,-22656,-27086,-24850,-15439,-1056,14573,27411,
    32767,28886,16858,257,-15834,-25881,-25649,-13969,4575,22655,
    32425,29057,14631,-5052,-21426,-27205,-18242,1334,21785,32500,
    27742,10559,-10457,-25069,-25196,-9741,12620,29656,31422,17698,
    -3698,-21801,-26928,-15293,6513,26256,32666,22734,2727,-17225,
    -27078,-21205,-2352,18917,31814,29590,14716,-5479,-21816,-27186,
    -18567,-231,19124,31311,31064,19819,2510,-14443,-25305,-26432,
    -17318,-1570,15031,27842,32733,29295,19300,5793,-8023,-19275,
    -25930,-26975,-22493,-13631,-2452,8748,19034,26779,31398,32723,
    31392,27767,22860,16870,10957,4333,-2030
};
const int16_t wave1[257] = {
    -590,-7751,-14537,-20301,-24728,-27682,-29329,-29965,-29985,-29722,
    -29457,-29324,-29362,-29529,-29736,-29906,-29985,-29986,-29948,-29942,
    -29961,-29942,-29570,-28379,-25558,-20305,-11884,-778,11881,23992,
    31626,32024,24624,11283,-4395,-18345,-27325,-29875,-26398,-18790,
    -9438,-565,6836,12739,17036,19937,21769,22820,23353,23576,
    23694,23879,24307,25100,26328,27943,29780,31505,32648,32644,
    30966,27264,21513,14056,5600,-2981,-10838,-17345,-22210,-25480,
    -27440,-28457,-28864,-28841,-28385,-27322,-25377,-22276,-17864,-12211,
    -5641,1309,8027,13970,18778,22309,24609,25805,25999,25181,
    23202,19809,14750,7956,-313,-9345,-18027,-25038,-29181,-29726,
    -26652,-20643,-12903,-4789,2546,8864,14074,18048,20981,23084,
    24625,25795,26811,27796,28886,30085,31337,32371,32767,31847,
    28856,23062,14172,2597,-10164,-21658,-28860,-29220,-21770,-8298,
    6909,20497,29444,32666,31324,27139,21911,16870,12827,10092,
    8781,8769,9913,11936,14574,17485,20375,22968,25101,26677,
    27686,28148,28069,27400,26029,23800,20553,16213,10833,4662,
    -1903,-8352,-14210,-19102,-22856,-25483,-27154,-28081,-28480,-28455,
    -28023,-27074,-25462,-23046,-19778,-15745,-11200,-6508,-2085,1673,
    4460,6031,6229,4896,1935,-2719,-8875,-16028,-23059,-28391,
    -30025,-26401,-16925,-3422,10982,23511,31008,32513,28921,21968,
    13536,5077,-2457,-8689,-13546,-17228,-19955,-22025,-23638,-25018,
    -26271,-27492,-28640,-29601,-30059,-29565,-27478,-23134,-16048,-6355,
    4780,16327,26128,31782,32295,27878,19886,10105,322,-8219,
    -14855,-19512,-22434,-23989,-24473,-24032,-22656,-20231,-16659,-11943,
    -6271,-22,6301,12193,17237,21210,24060,25869,26771,26862,
    26137,24505,21802,17872,12696,6391,-590
};
const int16_t wave2[257] = {
    -702,-7414,-12883,-15766,-17386,-16508,-14121,-9668,-3183,4425,
    15154,24964,32767,10542,-29620,-26580,-7249,11205,32767,6186,
    -32766,-10519,9467,32767,-8559,-30280,-2265,21332,26574,-27899,
    -20726,1640,25247,27423,-20476,-30385,-10975,3315,20051,29725,
    31807,11966,-15899,-28860,-31421,-32461,-31895,-32408,-31198,-27417,
    -11355,18232,32397,28459,15807,-85,-16601,-32652,-5855,32767,
    13035,-3844,-32766,310,31547,6700,-21698,-21906,30798,14249,
    -12080,-31608,17097,27542,-1092,-22641,-26719,26435,26200,8174,
    -10732,-24980,-32766,-10813,23607,32696,30637,26703,23133,21446,
    21273,23230,26412,30721,32444,24818,-9053,-32340,-26348,-12153,
    5757,23712,30528,-17431,-29635,-4372,16877,31728,-20577,-24791,
    1197,27491,16261,-32766,-12125,7155,32767,5357,-32766,-16752,
    1933,20373,32767,15998,-25242,-31477,-25569,-16317,-10098,-3974,
    -1654,808,-856,-1978,-7744,-12371,-22273,-28229,-32766,-2349,
    31276,26884,8500,-9397,-31360,-11081,32767,12696,-7332,-32766,
    11121,28187,643,-28248,-9218,32767,6666,-17287,-28877,24801,
    23920,-1070,-21738,-30870,15759,32436,16890,2174,-11276,-23339,
    -30289,-32168,-25649,-17444,-8556,-7205,-10608,-20312,-28031,-32450,
    -28798,-20082,-7945,6249,20475,32767,5308,-32766,-17805,1386,
    26406,22681,-29715,-17694,4591,32725,3185,-32766,-6916,15055,
    32767,-14175,-30443,-4367,12391,32767,13404,-28856,-28197,-14109,
    -1880,9841,18807,26333,30369,32257,32752,32502,32626,30448,
    27317,19488,11301,-1035,-12555,-27667,-29490,10762,32767,11325,
    -4189,-32682,-7613,32767,10925,-14138,-30530,22569,22013,-5882,
    -31927,5060,31643,1598,-21325,-25847,27817,23551,1474,-17482,
    -32766,-9643,30511,29391,19018,8207,-702
};
const int16_t wave3[257] = {
    -780,-6201,-10938,-14849,-18034,-20471,-22293,-23492,-24231,-24437,
    -24265,-23552,-22426,-20657,-18382,-15333,-11666,-7184,-2152,3493,
    9431,15487,21181,26204,30034,32292,32675,30806,26879,20673,
    13294,4037,-1153,-7573,-16371,-22635,-27951,-30981,-32668,-32540,
    -31435,-29256,-26645,-23646,-20720,-17914,-15508,-13535,-12101,-11270,
    -11038,-11461,-12470,-14114,-16256,-18896,-21817,-24908,-27842,-30415,
    -32162,-32767,-31777,-28934,-23951,-16913,-8152,1520,11335,20290,
    27321,31590,32738,30747,25918,18837,10257,998,-8082,-16316,
    -23189,-28268,-31447,-32720,-32319,-30518,-27714,-24274,-20572,-16935,
    -13609,-10744,-8492,-6905,-6026,-5856,-6397,-7637,-9542,-12069,
    -15102,-18510,-22109,-25623,-28752,-31173,-32550,-32623,-31153,-28052,
    -23323,-17176,-9902,-1876,6339,14208,21191,26776,30646,32570,
    32462,30472,26830,21913,16175,10089,4100,-1407,-6371,-10672,
    -14204,-16991,-19054,-20474,-21304,-21577,-21299,-20461,-19013,-16893,
    -14011,-10339,-5830,-619,5209,11538,17898,23754,28542,31682,
    32767,31475,27732,21670,13731,4543,-5192,-14537,-22630,-28687,
    -32109,-32593,-30095,-24907,-17612,-9048,-240,8207,15760,21937,
    26595,29772,31686,32592,32753,32441,31885,31269,30739,30398,
    30300,30457,30844,31391,32002,32521,32767,32527,31562,29639,
    26550,22139,16429,9506,1750,-6352,-14404,-21613,-27331,-31140,
    -32725,-32042,-29227,-24618,-18612,-11679,-4356,2918,9729,15762,
    20891,24995,28085,30253,31630,32391,32715,32763,32679,32580,
    32527,32558,32651,32748,32746,32503,31833,30554,28452,25369,
    21190,15833,9385,2065,-5805,-13670,-20978,-27055,-31181,-32766,
    -31379,-26927,-19595,-10101,370,10749,19941,26924,31262,32755,
    31700,28475,23702,17864,11556,5207,-780
};
const int16_t wave4[257] = {
    -109,-1333,-3212,-4948,-6820,-8544,-10250,-11755,-13124,-14253,
    -15189,-15886,-16389,-16682,-16804,-16776,-16637,-16418,-16167,-15937,
    -15759,-15689,-15750,-15992,-16410,-17038,-17845,-18828,-19947,-21181,
    -22460,-23753,-24994,-26153,-27199,-28112,-28875,-29490,-29968,-30324,
    -30574,-30726,-30799,-30809,-30769,-30692,-30590,-30486,-30400,-30348,
    -30354,-30425,-30575,-30800,-31089,-31424,-31779,-32120,-32415,-32632,
    -32750,-32758,-32659,-32466,-32201,-31897,-31583,-31296,-31055,-30878,
    -30776,-30742,-30765,-30831,-30925,-31020,-31107,-31164,-31180,-31143,
    -31037,-30852,-30569,-30171,-29639,-28964,-28138,-27155,-26032,-24794,
    -23476,-22137,-20818,-19572,-18459,-17504,-16746,-16195,-15845,-15688,
    -15698,-15837,-16060,-16322,-16573,-16769,-16863,-16816,-16602,-16180,
    -15540,-14674,-13570,-12248,-10719,-9015,-7197,-5310,-3423,-1605,
    96,1655,3044,4226,5168,5875,6345,6593,6649,6544,
    6332,6054,5763,5515,5361,5348,5515,5890,6501,7349,
    8428,9725,11206,12822,14522,16235,17915,19507,20960,22250,
    23349,24250,24963,25490,25849,26059,26136,26107,25996,25832,
    25643,25467,25342,25294,25356,25547,25881,26356,26950,27655,
    28425,29218,29992,30701,31323,31835,32222,32494,32657,32741,
    32767,32761,32742,32719,32708,32709,32715,32730,32742,32752,
    32761,32764,32763,32756,32740,32705,32637,32513,32314,32018,
    31611,31083,30438,29690,28867,28009,27149,26333,25604,24984,
    24508,24174,23987,23931,23988,24120,24305,24495,24662,24773,
    24797,24706,24481,24103,23552,22819,21895,20790,19508,18073,
    16517,14890,13232,11624,10075,8677,7439,6422,5618,5055,
    4698,4558,4571,4734,4971,5257,5520,5724,5810,5745,
    5498,5019,4340,3375,2269,773,-109
};
const int16_t wave5[257] = {
    -469,-2517,-5026,-7945,-11199,-14678,-18276,-21785,-25034,-27815,
    -29946,-31303,-31787,-31384,-30146,-28198,-25711,-22864,-19880,-16941,
    -14212,-11829,-9850,-8326,-7278,-6693,-6545,-6785,-7366,-8213,
    -9259,-10423,-11614,-12761,-13800,-14658,-15292,-15658,-15730,-15479,
    -14886,-13917,-12545,-10742,-8468,-5682,-2389,1421,5618,9950,
    14291,18431,22208,25470,28148,30193,31600,32432,32767,32733,
    32439,32006,31537,31113,30798,30626,30608,30732,30967,31266,
    31569,31813,31939,31893,31637,31165,30483,29625,28648,27624,
    26629,25739,25008,24481,24179,24081,24143,24280,24380,24309,
    23915,23063,21637,19549,16766,13346,9358,4988,399,-4239,
    -8718,-12876,-16581,-19800,-22471,-24612,-26255,-27446,-28253,-28721,
    -28899,-28817,-28499,-27966,-27223,-26287,-25184,-23937,-22596,-21199,
    -19801,-18464,-17252,-16215,-15414,-14878,-14639,-14713,-15095,-15764,
    -16669,-17753,-18919,-20061,-21055,-21771,-22096,-21931,-21209,-19910,
    -18076,-15783,-13133,-10292,-7415,-4659,-2181,-74,1573,2696,
    3269,3287,2777,1765,312,-1507,-3594,-5869,-8221,-10551,
    -12777,-14819,-16645,-18244,-19597,-20731,-21660,-22405,-22986,-23390,
    -23590,-23534,-23152,-22345,-21010,-19039,-16359,-12921,-8791,-4025,
    965,5692,9934,13466,16191,18087,19181,19578,19419,18867,
    18083,17210,16380,15681,15187,14933,14931,15174,15637,16280,
    17050,17906,18795,19679,20521,21302,22021,22678,23281,23839,
    24363,24849,25286,25638,25860,25890,25655,25081,24104,22681,
    20785,18451,15716,12658,9410,6084,2824,-243,-3053,-5525,
    -7614,-9322,-10655,-11646,-12335,-12744,-12904,-12823,-12517,-11977,
    -11209,-10215,-9008,-7621,-6097,-4480,-2854,-1291,132,1334,
    2262,2854,3065,2855,2207,1090,-469
};
const int16_t wave6[257] = {
    -5765,-15876,-17415,-18277,-15699,-16702,-19769,-23119,-26519,-24335,
    -18351,-16577,-16071,-21614,-21558,-18770,-10207,1922,5385,5561,
    5282,5885,2662,-537,-3983,-11456,-19483,-24544,-27696,-23791,
    -19416,-7296,1799,7748,11556,10574,11052,11545,16917,15830,
    17601,21951,21751,21977,21620,22312,18365,15988,16149,10701,
    11232,10504,12169,16486,16337,16352,20570,22540,17998,15925,
    16834,14981,10701,11042,10860,6494,5040,5766,634,-88,
    11,8,-10,4,9,-27,58,-96,161,-275,
    550,-3721,-1825,-4273,-6032,-5055,-5777,-5195,-5683,-5267,
    -5623,-5321,-5566,-5401,-5278,-248,20,-5,25,-61,
    133,-301,3974,5887,5209,5640,5316,5590,5314,5706,
    4417,-459,455,-641,2155,5774,5388,5423,5573,5276,
    5729,5061,6145,2945,387,5706,5331,5494,5509,5307,
    5820,3868,-452,255,-172,128,-98,74,-52,29,
    0,-37,89,-97,5276,2143,-592,328,-206,238,
    5007,5402,5910,2532,802,6186,5035,5711,5356,5396,
    5832,3172,-514,211,4509,5724,5403,5349,5810,1264,
    -143,-258,1748,5963,5221,5491,566,-218,318,4672,
    5967,1181,-622,-5887,663,8936,11026,10811,5113,-244,
    173,-104,54,4430,5937,4148,-2749,-5757,-5534,-3644,
    3581,9620,6717,2168,-2219,-10552,-10464,-2863,-513,4744,
    11414,13049,17489,15372,12403,5847,-6432,-14958,-22239,-19936,
    -13188,-6995,-775,7858,15482,16335,16576,16100,21381,21009,
    16170,16477,16361,16350,16457,16266,16543,16155,16763,14754,
    7836,-2249,-13138,-17025,-12845,-6440,-5406,-2433,6819,19632,
    29036,32767,32152,26106,15338,7124,-5765
};
const int16_t wave7[257] = {
    -11516,-17754,-15570,-16967,-15922,-16758,-16066,-16649,-16154,-16577,
    -16215,-16525,-16262,-16483,-16298,-16451,-16327,-16424,-16351,-16403,
    -16370,-16384,-16390,-16366,-16409,-16345,-16431,-16322,-16456,-16296,
    -16484,-16266,-16516,-16232,-16549,-16198,-16583,-16166,-16613,-16138,
    -16637,-16121,-16647,-16118,-16640,-16137,-16606,-16186,-16538,-16279,
    -16414,-16441,-16202,-16731,-15790,-17359,-14687,-19907,17046,32767,
    30350,32767,31470,32767,31925,32767,32200,32767,32395,32767,
    32543,32767,32656,32767,32741,32760,32767,32709,32767,32678,
    32767,32665,32767,32663,32767,32672,32767,32685,32767,32700,
    32767,32714,32767,32725,32767,32727,32767,32722,32767,32710,
    32767,32688,32767,32659,32767,32622,32767,32576,32767,32522,
    32767,32444,32767,32280,32767,30648,16682,16256,16530,16214,
    16566,16190,16579,16189,16570,16208,16543,16245,16494,16305,
    16416,16406,16284,16594,15984,17214,13240,-558,232,14,
    -272,654,-1447,25084,32767,31424,32767,32008,32767,32240,
    32767,32374,32767,32469,32767,32543,32767,32608,32767,32676,
    32767,32767,32690,32767,18942,15559,16901,15983,16722,16080,
    16664,16118,16640,16132,16632,16132,16638,16121,16655,16094,
    16693,16038,16784,15873,17175,13982,14,29,22,-59,
    87,-106,124,-137,152,-165,180,-194,214,-235,
    264,-301,353,-430,559,-801,1434,-5799,-17765,-15663,
    -16845,-16056,-16617,-16214,-16494,-16320,-16396,-16416,-16296,-16526,
    -16171,-16675,-15989,-16909,-15666,-17396,-14811,-19392,-1875,19622,
    14610,17618,15391,17354,14505,787,-298,193,-132,88,
    -50,22,3,-21,34,-42,45,-40,29,-8,
    -22,69,-140,257,-477,1034,-11516
};
const int16_t wave8[257] = {
    -141,-1500,-2860,-4212,-5558,-6895,-8220,-9528,-10821,-12092,
    -13344,-14571,-15772,-16945,-18088,-19198,-20276,-21315,-22318,-23283,
    -24205,-25088,-25925,-26719,-27465,-28166,-28818,-29422,-29978,-30483,
    -30938,-31342,-31697,-31999,-32253,-32454,-32608,-32708,-32763,-32767,
    -32725,-32635,-32499,-32318,-32095,-31829,-31523,-31176,-30794,-30374,
    -29921,-29436,-28920,-28378,-27806,-27215,-26595,-25967,-25308,-24651,
    -23961,-23286,-22567,-21911,-20920,-19524,-18184,-16818,-15481,-14139,
    -12820,-11508,-10218,-8943,-7692,-6464,-5262,-4092,-2948,-1839,
    -760,279,1283,2245,3171,4049,4889,5680,6429,7127,
    7782,8385,8941,9446,9901,10305,10659,10963,11215,11419,
    11570,11672,11724,11731,11688,11598,11462,11282,11057,10793,
    10486,10140,9757,9337,8884,8400,7883,7339,6770,6177,
    5560,4927,4274,3608,2929,2242,1546,843,141,-565,
    -1265,-1961,-2649,-3327,-3995,-4646,-5281,-5895,-6489,-7059,
    -7601,-8119,-8602,-9057,-9476,-9859,-10205,-10511,-10777,-11001,
    -11182,-11317,-11406,-11449,-11445,-11391,-11289,-11136,-10933,-10681,
    -10377,-10025,-9618,-9165,-8657,-8104,-7499,-6847,-6146,-5400,
    -4605,-3768,-2886,-1966,-1000,2,1044,2120,3231,4372,
    5544,6749,7972,9228,10495,11794,13095,14428,15753,17115,
    18446,19846,20922,21590,22309,22993,23691,24364,25035,25684,
    26321,26934,27529,28099,28641,29158,29642,30097,30514,30899,
    31243,31551,31815,32039,32219,32355,32444,32489,32482,32431,
    32327,32175,31972,31719,31415,31063,30658,30202,29697,29142,
    28539,27886,27185,26438,25646,24808,23926,23002,22039,21035,
    19995,18919,17807,16665,15490,14291,13063,11811,10540,9247,
    7938,6613,5279,3932,2578,1220,-141
};
const int16_t wave9[257] = {
    -14540,-32767,-28555,-32142,-28878,-30860,-28596,-29856,-28150,-28941,
    -27627,-28062,-27057,-27199,-26442,-26348,-25791,-25498,-25096,-24648,
    -24360,-23796,-23584,-22939,-22764,-22066,-21905,-21172,-21128,-20820,
    -20955,-20660,-20738,-20473,-20478,-20259,-20151,-19645,-19085,-18576,
    -17937,-17469,-16745,-16318,-15510,-15286,-14933,-14972,-14565,-14597,
    -14164,-14164,-13729,-13668,-13262,-13105,-12768,-12466,-12262,-11725,
    -11782,-10805,-11513,-9160,-24018,-32767,-29759,-30989,-29376,-29846,
    -28666,-28784,-27833,-27716,-26919,-26624,-25938,-25498,-24898,-24335,
    -23802,-23131,-22657,-21790,-20857,-19586,-18662,-17338,-16416,-15056,
    -14121,-12731,-11904,-10937,-10287,-9333,-8619,-7699,-6906,-6043,
    -5124,-3997,-2610,-1510,-15,998,2609,3532,5259,6096,
    7929,8688,10608,11314,13292,13976,15972,16679,18636,19432,
    21267,22251,23838,25175,26279,28318,28369,31564,-14541,-32767,
    -26506,-29070,-24785,-25749,-22469,-22718,-20004,-19794,-17483,-16926,
    -14937,-14104,-12376,-11321,-9813,-8579,-7245,-5878,-4679,-3217,
    -2119,-602,431,1974,2965,4515,5358,6452,7085,8130,
    8786,9764,10458,11354,12126,13270,14448,15559,16776,17805,
    19064,20009,21312,22007,22808,23196,24008,24355,25146,25479,
    26224,26570,27237,27631,28181,28671,29038,29712,29767,30832,
    30187,32579,17732,8883,11941,10647,12175,11592,12634,12353,
    13114,13018,13581,13613,14015,14146,14413,14619,14771,15039,
    15086,15503,15964,16740,17149,17934,18299,19080,19414,20184,
    20368,20676,20650,20905,20904,21090,21133,21227,21362,21689,
    22262,22530,23181,23335,24074,24112,24941,24868,25778,25609,
    26587,26341,27358,27072,28092,27815,28781,28580,29413,29389,
    29964,30284,30372,31388,30419,32589,-14540
};
const int16_t wave10[257] = {
    -110,-1956,-3818,-5675,-7500,-9218,-10873,-12468,-14005,-15457,
    -16807,-18033,-19204,-20274,-21177,-22144,-22757,-23264,-23868,-24230,
    -24405,-24524,-24573,-24391,-24135,-23931,-23538,-23104,-22645,-22046,
    -21339,-20645,-19985,-19118,-18242,-17277,-16309,-15221,-14149,-13053,
    -11839,-10626,-9404,-8152,-6927,-5728,-4488,-3245,-2003,-752,
    461,1651,2807,3798,4962,5898,6880,7794,8686,9588,
    10442,11299,12045,12935,13703,14458,15204,15974,16720,17502,
    18189,18861,19376,19985,20549,21036,21614,22174,22747,23283,
    23833,24358,25022,25569,26159,26746,27315,27874,28502,29071,
    29526,30068,30594,30973,31327,31706,32008,32270,32466,32659,
    32671,32767,32733,32498,32366,32079,31813,31454,31028,30581,
    29985,29394,28767,28007,27242,26437,25537,24627,23639,22635,
    21579,20513,19447,18375,17347,16328,15364,14426,13532,12687,
    11892,11186,10506,9844,9189,8731,8262,7855,7506,7183,
    6922,6616,6486,6365,6315,6267,6358,6519,6618,6832,
    7096,7348,7623,7956,8175,8501,8821,8965,9128,9192,
    9245,9205,9112,8937,8745,8506,8225,7855,7492,7108,
    6612,6212,5757,5372,4885,4421,3896,3381,2863,2265,
    1758,1200,630,42,-598,-1208,-1784,-2327,-2878,-3404,
    -3842,-4285,-4651,-4922,-5110,-5265,-5345,-5257,-5084,-4880,
    -4464,-4053,-3602,-3024,-2382,-1664,-898,-97,756,1697,
    2678,3721,4786,5916,7059,8303,9564,10848,12132,13475,
    14862,16083,17404,18632,19837,20971,21987,23000,23834,24691,
    25298,25822,26223,26410,26571,26485,26329,26001,25504,24922,
    24222,23406,22384,21291,20017,18698,17320,15783,14230,12614,
    10906,9108,7326,5492,3611,1750,-110
};
const int16_t wave11[257] = {
    -1236,-4419,-7808,-11097,-14668,-18373,-21445,-23959,-26452,-28798,
    -30680,-31796,-31688,-30798,-30870,-31719,-32269,-32767,-32386,-30744,
    -29080,-27997,-27376,-27326,-27238,-26273,-24753,-22734,-20214,-18544,
    -18244,-17825,-16372,-14976,-13687,-12571,-12403,-12466,-12190,-11320,
    -10131,-9009,-8489,-8502,-8679,-8759,-8102,-6818,-5186,-3799,
    -2452,-1088,49,1601,3765,6210,8565,11351,14201,16548,
    18739,20687,22006,23084,24774,26577,28216,29367,29781,29718,
    29102,28311,27768,27597,26950,25665,24643,23541,21279,18387,
    15892,12854,9474,6444,3746,1451,-616,-2634,-5132,-7959,
    -11050,-13777,-15299,-16624,-18799,-20700,-21447,-21555,-21013,-20373,
    -20591,-21549,-22579,-23079,-22602,-21414,-20585,-20639,-20634,-20565,
    -20401,-19050,-16719,-14649,-13530,-13122,-13194,-12968,-11887,-10359,
    -9084,-8525,-8479,-8428,-7787,-6863,-6055,-5468,-5458,-5982,
    -6478,-6041,-5067,-3982,-2521,-1007,153,1450,3251,4972,
    7068,9273,11046,13026,15410,17715,19770,21782,23409,25278,
    27481,29310,30833,31900,31841,30545,29786,29499,28731,28004,
    27512,26975,25447,23503,21701,19609,16971,13958,11187,8622,
    5970,3547,2186,588,-2168,-4807,-6674,-7992,-9341,-10594,
    -11810,-12917,-13118,-12487,-11592,-11080,-11625,-12888,-13443,-12853,
    -11970,-11047,-10019,-9597,-9724,-9498,-8844,-7718,-6048,-4836,
    -4558,-4825,-5184,-4773,-3324,-1936,-1758,-2198,-2553,-2955,
    -3011,-2415,-2115,-2846,-3582,-4429,-5466,-5747,-5493,-4835,
    -3935,-3155,-2740,-1834,-393,1262,3409,5536,7629,10058,
    13081,15632,17926,20570,22927,25161,27622,30224,31725,32092,
    32179,32183,31647,30999,30977,30909,30388,29052,27437,25498,
    22220,18219,14498,10773,6279,1989,-1236
};
const int16_t wave12[257] = {
    -128,-1123,-2123,-3135,-4159,-5198,-6249,-7297,-8322,-9298,
    -10219,-11079,-11882,-12635,-13356,-14058,-14748,-15409,-16024,-16579,
    -17073,-17508,-17892,-18246,-18584,-18916,-19232,-19521,-19775,-19991,
    -20168,-20317,-20449,-20569,-20670,-20745,-20789,-20802,-20787,-20754,
    -20724,-20707,-20713,-20748,-20815,-20910,-21017,-21125,-21237,-21359,
    -21494,-21658,-21861,-22099,-22360,-22625,-22883,-23128,-23363,-23599,
    -23841,-24085,-24322,-24548,-24751,-24928,-25086,-25221,-25326,-25403,
    -25450,-25464,-25428,-25328,-25169,-24936,-24631,-24264,-23847,-23385,
    -22876,-22314,-21692,-20998,-20223,-19380,-18470,-17506,-16498,-15460,
    -14396,-13305,-12186,-11047,-9890,-8714,-7538,-6373,-5230,-4109,
    -3024,-1977,-980,-36,822,1572,2217,2759,3197,3544,
    3817,4029,4198,4328,4430,4511,4575,4630,4690,4775,
    4904,5084,5305,5568,5867,6197,6543,6907,7297,7714,
    8159,8630,9129,9654,10195,10751,11317,11895,12488,13101,
    13735,14388,15064,15745,16426,17101,17771,18438,19105,19779,
    20462,21144,21821,22482,23125,23750,24352,24943,25526,26094,
    26650,27196,27730,28246,28741,29210,29647,30055,30434,30787,
    31111,31405,31670,31903,32106,32280,32433,32558,32656,32725,
    32763,32767,32734,32668,32573,32453,32306,32136,31941,31720,
    31470,31188,30873,30523,30140,29722,29272,28790,28271,27713,
    27114,26482,25826,25152,24470,23781,23090,22391,21677,20941,
    20190,19433,18686,17955,17252,16573,15920,15286,14672,14088,
    13533,13012,12532,12091,11681,11301,10955,10657,10407,10207,
    10059,9949,9859,9775,9675,9546,9384,9202,9012,8827,
    8655,8489,8308,8084,7799,7446,7023,6533,5995,5422,
    4807,4141,3418,2628,1771,849,-128
};

#endif