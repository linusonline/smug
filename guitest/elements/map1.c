#include <common.h>
#include <graphics/sprite.h>
#include <graphics/spritesheet.h>
#include <engine/gameobject.h>

static const char* dirtFile = "res/inquisitor/Inq XP MT - Dirt.png";
// static const char* grassFile = "res/inquisitor/Inq XP MT - Long Grass2.png";
// static const char* mudFile = "res/inquisitor/Inq XP MT - Mud.png";
// static const char* roadFile1 = "res/inquisitor/Inq XP MT - Road.png";
// static const char* roadFile2 = "res/inquisitor/Inq XP MT - Road 2.png";
static const char* dataFile = "res/inquisitor/inquisitor-tiles.txt";

static SpriteSheet* dirtSheet = NULL;
// static SpriteSheet* grassSheet = NULL;
// static SpriteSheet* mudSheet = NULL;
// static SpriteSheet* roadSheet1 = NULL;
// static SpriteSheet* roadSheet2 = NULL;

static GameObject** world = NULL;

static const int TILE_WIDTH = 32;
static const int TILE_HEIGHT = 32;

static const int MAP_WIDTH = 20;
static const int MAP_HEIGHT = 15;

#define sprite(type, xi, yi) SpriteSheet_getSpriteXY(type, xi, yi)
#define landtile(s, x, y) GameObject_newWithDrawable(x * TILE_WIDTH, y * TILE_HEIGHT, Drawable_newFromSpriteAndSize(s, TILE_WIDTH, TILE_HEIGHT), 0.0f, 0.0f)
#define GRASS_SPRITE sprite(dirtSprites, 0, 0)

int map1Size()
{
    return MAP_WIDTH * MAP_HEIGHT;
}

void deleteMap1()
{
    for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++)
    {
        GameObject_delete(world[i]);
    }
    free(world);

    SpriteSheet_delete(dirtSheet);
    // SpriteSheet_delete(grassSheet);
    // SpriteSheet_delete(mudSheet);
    // SpriteSheet_delete(roadSheet1);
    // SpriteSheet_delete(roadSheet2);
}

GameObject** createMap1()
{
    dirtSheet = SpriteSheet_new(dirtFile, dataFile);
    // grassSheet = SpriteSheet_new(grassFile, dataFile);
    // mudSheet = SpriteSheet_new(mudFile, dataFile);
    // roadSheet1 = SpriteSheet_new(roadFile1, dataFile);
    // roadSheet2 = SpriteSheet_new(roadFile2, dataFile);

    world = allocatev(GameObject*, MAP_WIDTH * MAP_HEIGHT);

    world[0] =  landtile(sprite(dirtSheet, 0, 0), 0, 0);
    world[1] =  landtile(sprite(dirtSheet, 0, 0), 1, 0);
    world[2] =  landtile(sprite(dirtSheet, 0, 0), 2, 0);
    world[3] =  landtile(sprite(dirtSheet, 0, 0), 3, 0);
    world[4] =  landtile(sprite(dirtSheet, 0, 0), 4, 0);
    world[5] =  landtile(sprite(dirtSheet, 0, 0), 5, 0);
    world[6] =  landtile(sprite(dirtSheet, 0, 1), 6, 0);
    world[7] =  landtile(sprite(dirtSheet, 1, 1), 7, 0);
    world[8] =  landtile(sprite(dirtSheet, 1, 1), 8, 0);
    world[9] =  landtile(sprite(dirtSheet, 1, 1), 9, 0);
    world[10] = landtile(sprite(dirtSheet, 0, 4), 10, 0);
    world[11] = landtile(sprite(dirtSheet, 1, 2), 11, 0);
    world[12] = landtile(sprite(dirtSheet, 1, 2), 12, 0);
    world[13] = landtile(sprite(dirtSheet, 1, 2), 13, 0);
    world[14] = landtile(sprite(dirtSheet, 1, 2), 14, 0);
    world[15] = landtile(sprite(dirtSheet, 1, 4), 15, 0);
    world[16] = landtile(sprite(dirtSheet, 1, 1), 16, 0);
    world[17] = landtile(sprite(dirtSheet, 1, 1), 17, 0);
    world[18] = landtile(sprite(dirtSheet, 1, 1), 18, 0);
    world[19] = landtile(sprite(dirtSheet, 2, 1), 19, 0);

    world[20] = landtile(sprite(dirtSheet, 0, 0), 0, 1);
    world[21] = landtile(sprite(dirtSheet, 0, 0), 1, 1);
    world[22] = landtile(sprite(dirtSheet, 0, 0), 2, 1);
    world[23] = landtile(sprite(dirtSheet, 0, 1), 3, 1);
    world[24] = landtile(sprite(dirtSheet, 1, 1), 4, 1);
    world[25] = landtile(sprite(dirtSheet, 1, 1), 5, 1);
    world[26] = landtile(sprite(dirtSheet, 0, 4), 6, 1);
    world[27] = landtile(sprite(dirtSheet, 1, 2), 7, 1);
    world[28] = landtile(sprite(dirtSheet, 1, 2), 8, 1);
    world[29] = landtile(sprite(dirtSheet, 1, 2), 9, 1);
    world[30] = landtile(sprite(dirtSheet, 1, 2), 10, 1);
    world[31] = landtile(sprite(dirtSheet, 1, 2), 11, 1);
    world[32] = landtile(sprite(dirtSheet, 1, 2), 12, 1);
    world[33] = landtile(sprite(dirtSheet, 1, 2), 13, 1);
    world[34] = landtile(sprite(dirtSheet, 1, 2), 14, 1);
    world[35] = landtile(sprite(dirtSheet, 1, 2), 15, 1);
    world[36] = landtile(sprite(dirtSheet, 1, 2), 16, 1);
    world[37] = landtile(sprite(dirtSheet, 1, 2), 17, 1);
    world[38] = landtile(sprite(dirtSheet, 1, 2), 18, 1);
    world[39] = landtile(sprite(dirtSheet, 2, 2), 19, 1);

    world[40] = landtile(sprite(dirtSheet, 0, 0), 0, 2);
    world[41] = landtile(sprite(dirtSheet, 0, 0), 1, 2);
    world[42] = landtile(sprite(dirtSheet, 0, 0), 2, 2);
    world[43] = landtile(sprite(dirtSheet, 0, 2), 3, 2);
    world[44] = landtile(sprite(dirtSheet, 1, 2), 4, 2);
    world[45] = landtile(sprite(dirtSheet, 1, 2), 5, 2);
    world[46] = landtile(sprite(dirtSheet, 1, 2), 6, 2);
    world[47] = landtile(sprite(dirtSheet, 1, 2), 7, 2);
    world[48] = landtile(sprite(dirtSheet, 1, 2), 8, 2);
    world[49] = landtile(sprite(dirtSheet, 1, 2), 9, 2);
    world[50] = landtile(sprite(dirtSheet, 1, 2), 10, 2);
    world[51] = landtile(sprite(dirtSheet, 1, 2), 11, 2);
    world[52] = landtile(sprite(dirtSheet, 1, 2), 12, 2);
    world[53] = landtile(sprite(dirtSheet, 1, 2), 13, 2);
    world[54] = landtile(sprite(dirtSheet, 1, 2), 14, 2);
    world[55] = landtile(sprite(dirtSheet, 1, 2), 15, 2);
    world[56] = landtile(sprite(dirtSheet, 1, 2), 16, 2);
    world[57] = landtile(sprite(dirtSheet, 1, 2), 17, 2);
    world[58] = landtile(sprite(dirtSheet, 1, 2), 18, 2);
    world[59] = landtile(sprite(dirtSheet, 1, 4), 19, 2);

    world[60] = landtile(sprite(dirtSheet, 1, 1), 0, 3);
    world[61] = landtile(sprite(dirtSheet, 1, 1), 1, 3);
    world[62] = landtile(sprite(dirtSheet, 1, 1), 2, 3);
    world[63] = landtile(sprite(dirtSheet, 0, 4), 3, 3);
    world[64] = landtile(sprite(dirtSheet, 1, 2), 4, 3);
    world[65] = landtile(sprite(dirtSheet, 1, 2), 5, 3);
    world[66] = landtile(sprite(dirtSheet, 1, 2), 6, 3);
    world[67] = landtile(sprite(dirtSheet, 1, 2), 7, 3);
    world[68] = landtile(sprite(dirtSheet, 1, 2), 8, 3);
    world[69] = landtile(sprite(dirtSheet, 1, 2), 9, 3);
    world[70] = landtile(sprite(dirtSheet, 1, 2), 10, 3);
    world[71] = landtile(sprite(dirtSheet, 1, 2), 11, 3);
    world[72] = landtile(sprite(dirtSheet, 1, 2), 12, 3);
    world[73] = landtile(sprite(dirtSheet, 1, 2), 13, 3);
    world[74] = landtile(sprite(dirtSheet, 1, 2), 14, 3);
    world[75] = landtile(sprite(dirtSheet, 1, 2), 15, 3);
    world[76] = landtile(sprite(dirtSheet, 1, 2), 16, 3);
    world[77] = landtile(sprite(dirtSheet, 1, 2), 17, 3);
    world[78] = landtile(sprite(dirtSheet, 1, 2), 18, 3);
    world[79] = landtile(sprite(dirtSheet, 1, 2), 19, 3);

    world[80] = landtile(sprite(dirtSheet, 1, 2), 0, 4);
    world[81] = landtile(sprite(dirtSheet, 1, 2), 1, 4);
    world[82] = landtile(sprite(dirtSheet, 1, 2), 2, 4);
    world[83] = landtile(sprite(dirtSheet, 1, 2), 3, 4);
    world[84] = landtile(sprite(dirtSheet, 1, 2), 4, 4);
    world[85] = landtile(sprite(dirtSheet, 1, 2), 5, 4);
    world[86] = landtile(sprite(dirtSheet, 1, 2), 6, 4);
    world[87] = landtile(sprite(dirtSheet, 1, 2), 7, 4);
    world[88] = landtile(sprite(dirtSheet, 1, 2), 8, 4);
    world[89] = landtile(sprite(dirtSheet, 1, 2), 9, 4);
    world[90] = landtile(sprite(dirtSheet, 1, 2), 10, 4);
    world[91] = landtile(sprite(dirtSheet, 1, 2), 11, 4);
    world[92] = landtile(sprite(dirtSheet, 1, 2), 12, 4);
    world[93] = landtile(sprite(dirtSheet, 1, 2), 13, 4);
    world[94] = landtile(sprite(dirtSheet, 1, 2), 14, 4);
    world[95] = landtile(sprite(dirtSheet, 1, 2), 15, 4);
    world[96] = landtile(sprite(dirtSheet, 1, 2), 16, 4);
    world[97] = landtile(sprite(dirtSheet, 1, 2), 17, 4);
    world[98] = landtile(sprite(dirtSheet, 1, 2), 18, 4);
    world[99] = landtile(sprite(dirtSheet, 1, 2), 19, 4);

    world[100] = landtile(sprite(dirtSheet, 1, 2), 0, 5);
    world[101] = landtile(sprite(dirtSheet, 1, 2), 1, 5);
    world[102] = landtile(sprite(dirtSheet, 1, 2), 2, 5);
    world[103] = landtile(sprite(dirtSheet, 1, 2), 3, 5);
    world[104] = landtile(sprite(dirtSheet, 1, 2), 4, 5);
    world[105] = landtile(sprite(dirtSheet, 1, 2), 5, 5);
    world[106] = landtile(sprite(dirtSheet, 1, 2), 6, 5);
    world[107] = landtile(sprite(dirtSheet, 1, 2), 7, 5);
    world[108] = landtile(sprite(dirtSheet, 1, 2), 8, 5);
    world[109] = landtile(sprite(dirtSheet, 1, 2), 9, 5);
    world[110] = landtile(sprite(dirtSheet, 1, 2), 10, 5);
    world[111] = landtile(sprite(dirtSheet, 1, 2), 11, 5);
    world[112] = landtile(sprite(dirtSheet, 1, 2), 12, 5);
    world[113] = landtile(sprite(dirtSheet, 1, 2), 13, 5);
    world[114] = landtile(sprite(dirtSheet, 1, 2), 14, 5);
    world[115] = landtile(sprite(dirtSheet, 1, 2), 15, 5);
    world[116] = landtile(sprite(dirtSheet, 1, 2), 16, 5);
    world[117] = landtile(sprite(dirtSheet, 1, 2), 17, 5);
    world[118] = landtile(sprite(dirtSheet, 1, 2), 18, 5);
    world[119] = landtile(sprite(dirtSheet, 1, 2), 19, 5);

    world[120] = landtile(sprite(dirtSheet, 1, 2), 0, 6);
    world[121] = landtile(sprite(dirtSheet, 1, 2), 1, 6);
    world[122] = landtile(sprite(dirtSheet, 1, 2), 2, 6);
    world[123] = landtile(sprite(dirtSheet, 1, 2), 3, 6);
    world[124] = landtile(sprite(dirtSheet, 1, 2), 4, 6);
    world[125] = landtile(sprite(dirtSheet, 1, 2), 5, 6);
    world[126] = landtile(sprite(dirtSheet, 1, 2), 6, 6);
    world[127] = landtile(sprite(dirtSheet, 1, 2), 7, 6);
    world[128] = landtile(sprite(dirtSheet, 1, 2), 8, 6);
    world[129] = landtile(sprite(dirtSheet, 1, 2), 9, 6);
    world[130] = landtile(sprite(dirtSheet, 1, 2), 10, 6);
    world[131] = landtile(sprite(dirtSheet, 1, 2), 11, 6);
    world[132] = landtile(sprite(dirtSheet, 1, 2), 12, 6);
    world[133] = landtile(sprite(dirtSheet, 1, 2), 13, 6);
    world[134] = landtile(sprite(dirtSheet, 1, 2), 14, 6);
    world[135] = landtile(sprite(dirtSheet, 1, 2), 15, 6);
    world[136] = landtile(sprite(dirtSheet, 1, 2), 16, 6);
    world[137] = landtile(sprite(dirtSheet, 1, 2), 17, 6);
    world[138] = landtile(sprite(dirtSheet, 1, 2), 18, 6);
    world[139] = landtile(sprite(dirtSheet, 1, 2), 19, 6);

    world[140] = landtile(sprite(dirtSheet, 1, 2), 0, 7);
    world[141] = landtile(sprite(dirtSheet, 1, 2), 1, 7);
    world[142] = landtile(sprite(dirtSheet, 1, 2), 2, 7);
    world[143] = landtile(sprite(dirtSheet, 1, 2), 3, 7);
    world[144] = landtile(sprite(dirtSheet, 1, 2), 4, 7);
    world[145] = landtile(sprite(dirtSheet, 1, 2), 5, 7);
    world[146] = landtile(sprite(dirtSheet, 1, 2), 6, 7);
    world[147] = landtile(sprite(dirtSheet, 1, 2), 7, 7);
    world[148] = landtile(sprite(dirtSheet, 1, 2), 8, 7);
    world[149] = landtile(sprite(dirtSheet, 1, 2), 9, 7);
    world[150] = landtile(sprite(dirtSheet, 1, 2), 10, 7);
    world[151] = landtile(sprite(dirtSheet, 1, 2), 11, 7);
    world[152] = landtile(sprite(dirtSheet, 1, 2), 12, 7);
    world[153] = landtile(sprite(dirtSheet, 1, 2), 13, 7);
    world[154] = landtile(sprite(dirtSheet, 1, 2), 14, 7);
    world[155] = landtile(sprite(dirtSheet, 1, 2), 15, 7);
    world[156] = landtile(sprite(dirtSheet, 1, 2), 16, 7);
    world[157] = landtile(sprite(dirtSheet, 1, 2), 17, 7);
    world[158] = landtile(sprite(dirtSheet, 1, 2), 18, 7);
    world[159] = landtile(sprite(dirtSheet, 1, 2), 19, 7);

    world[160] = landtile(sprite(dirtSheet, 1, 2), 0, 8);
    world[161] = landtile(sprite(dirtSheet, 1, 2), 1, 8);
    world[162] = landtile(sprite(dirtSheet, 1, 2), 2, 8);
    world[163] = landtile(sprite(dirtSheet, 1, 2), 3, 8);
    world[164] = landtile(sprite(dirtSheet, 1, 2), 4, 8);
    world[165] = landtile(sprite(dirtSheet, 1, 2), 5, 8);
    world[166] = landtile(sprite(dirtSheet, 1, 2), 6, 8);
    world[167] = landtile(sprite(dirtSheet, 1, 2), 7, 8);
    world[168] = landtile(sprite(dirtSheet, 1, 2), 8, 8);
    world[169] = landtile(sprite(dirtSheet, 1, 2), 9, 8);
    world[170] = landtile(sprite(dirtSheet, 1, 2), 10, 8);
    world[171] = landtile(sprite(dirtSheet, 1, 2), 11, 8);
    world[172] = landtile(sprite(dirtSheet, 1, 2), 12, 8);
    world[173] = landtile(sprite(dirtSheet, 1, 2), 13, 8);
    world[174] = landtile(sprite(dirtSheet, 1, 2), 14, 8);
    world[175] = landtile(sprite(dirtSheet, 1, 2), 15, 8);
    world[176] = landtile(sprite(dirtSheet, 1, 2), 16, 8);
    world[177] = landtile(sprite(dirtSheet, 1, 2), 17, 8);
    world[178] = landtile(sprite(dirtSheet, 1, 2), 18, 8);
    world[179] = landtile(sprite(dirtSheet, 1, 2), 19, 8);

    world[180] = landtile(sprite(dirtSheet, 1, 2), 0, 9);
    world[181] = landtile(sprite(dirtSheet, 1, 2), 1, 9);
    world[182] = landtile(sprite(dirtSheet, 1, 2), 2, 9);
    world[183] = landtile(sprite(dirtSheet, 1, 2), 3, 9);
    world[184] = landtile(sprite(dirtSheet, 1, 2), 4, 9);
    world[185] = landtile(sprite(dirtSheet, 1, 2), 5, 9);
    world[186] = landtile(sprite(dirtSheet, 1, 2), 6, 9);
    world[187] = landtile(sprite(dirtSheet, 1, 2), 7, 9);
    world[188] = landtile(sprite(dirtSheet, 1, 2), 8, 9);
    world[189] = landtile(sprite(dirtSheet, 1, 2), 9, 9);
    world[190] = landtile(sprite(dirtSheet, 1, 2), 10, 9);
    world[191] = landtile(sprite(dirtSheet, 1, 2), 11, 9);
    world[192] = landtile(sprite(dirtSheet, 1, 2), 12, 9);
    world[193] = landtile(sprite(dirtSheet, 1, 2), 13, 9);
    world[194] = landtile(sprite(dirtSheet, 1, 2), 14, 9);
    world[195] = landtile(sprite(dirtSheet, 1, 2), 15, 9);
    world[196] = landtile(sprite(dirtSheet, 1, 2), 16, 9);
    world[197] = landtile(sprite(dirtSheet, 1, 2), 17, 9);
    world[198] = landtile(sprite(dirtSheet, 1, 2), 18, 9);
    world[199] = landtile(sprite(dirtSheet, 1, 2), 19, 9);

    world[200] = landtile(sprite(dirtSheet, 1, 2), 0, 10);
    world[201] = landtile(sprite(dirtSheet, 1, 2), 1, 10);
    world[202] = landtile(sprite(dirtSheet, 1, 2), 2, 10);
    world[203] = landtile(sprite(dirtSheet, 1, 2), 3, 10);
    world[204] = landtile(sprite(dirtSheet, 1, 2), 4, 10);
    world[205] = landtile(sprite(dirtSheet, 1, 2), 5, 10);
    world[206] = landtile(sprite(dirtSheet, 1, 2), 6, 10);
    world[207] = landtile(sprite(dirtSheet, 1, 2), 7, 10);
    world[208] = landtile(sprite(dirtSheet, 1, 2), 8, 10);
    world[209] = landtile(sprite(dirtSheet, 1, 2), 9, 10);
    world[210] = landtile(sprite(dirtSheet, 1, 2), 10, 10);
    world[211] = landtile(sprite(dirtSheet, 1, 2), 11, 10);
    world[212] = landtile(sprite(dirtSheet, 1, 2), 12, 10);
    world[213] = landtile(sprite(dirtSheet, 1, 2), 13, 10);
    world[214] = landtile(sprite(dirtSheet, 1, 2), 14, 10);
    world[215] = landtile(sprite(dirtSheet, 1, 2), 15, 10);
    world[216] = landtile(sprite(dirtSheet, 1, 2), 16, 10);
    world[217] = landtile(sprite(dirtSheet, 1, 2), 17, 10);
    world[218] = landtile(sprite(dirtSheet, 1, 2), 18, 10);
    world[219] = landtile(sprite(dirtSheet, 1, 2), 19, 10);

    world[220] = landtile(sprite(dirtSheet, 1, 2), 0, 11);
    world[221] = landtile(sprite(dirtSheet, 1, 2), 1, 11);
    world[222] = landtile(sprite(dirtSheet, 1, 2), 2, 11);
    world[223] = landtile(sprite(dirtSheet, 1, 2), 3, 11);
    world[224] = landtile(sprite(dirtSheet, 1, 2), 4, 11);
    world[225] = landtile(sprite(dirtSheet, 1, 2), 5, 11);
    world[226] = landtile(sprite(dirtSheet, 1, 2), 6, 11);
    world[227] = landtile(sprite(dirtSheet, 1, 2), 7, 11);
    world[228] = landtile(sprite(dirtSheet, 1, 2), 8, 11);
    world[229] = landtile(sprite(dirtSheet, 1, 2), 9, 11);
    world[230] = landtile(sprite(dirtSheet, 1, 2), 10, 11);
    world[231] = landtile(sprite(dirtSheet, 1, 2), 11, 11);
    world[232] = landtile(sprite(dirtSheet, 1, 2), 12, 11);
    world[233] = landtile(sprite(dirtSheet, 1, 2), 13, 11);
    world[234] = landtile(sprite(dirtSheet, 1, 2), 14, 11);
    world[235] = landtile(sprite(dirtSheet, 1, 2), 15, 11);
    world[236] = landtile(sprite(dirtSheet, 1, 2), 16, 11);
    world[237] = landtile(sprite(dirtSheet, 1, 2), 17, 11);
    world[238] = landtile(sprite(dirtSheet, 1, 2), 18, 11);
    world[239] = landtile(sprite(dirtSheet, 1, 2), 19, 11);

    world[240] = landtile(sprite(dirtSheet, 1, 2), 0, 12);
    world[241] = landtile(sprite(dirtSheet, 1, 2), 1, 12);
    world[242] = landtile(sprite(dirtSheet, 1, 2), 2, 12);
    world[243] = landtile(sprite(dirtSheet, 1, 2), 3, 12);
    world[244] = landtile(sprite(dirtSheet, 1, 2), 4, 12);
    world[245] = landtile(sprite(dirtSheet, 1, 2), 5, 12);
    world[246] = landtile(sprite(dirtSheet, 1, 2), 6, 12);
    world[247] = landtile(sprite(dirtSheet, 1, 2), 7, 12);
    world[248] = landtile(sprite(dirtSheet, 1, 2), 8, 12);
    world[249] = landtile(sprite(dirtSheet, 1, 2), 9, 12);
    world[250] = landtile(sprite(dirtSheet, 1, 2), 10, 12);
    world[251] = landtile(sprite(dirtSheet, 1, 2), 11, 12);
    world[252] = landtile(sprite(dirtSheet, 1, 2), 12, 12);
    world[253] = landtile(sprite(dirtSheet, 1, 2), 13, 12);
    world[254] = landtile(sprite(dirtSheet, 1, 2), 14, 12);
    world[255] = landtile(sprite(dirtSheet, 1, 2), 15, 12);
    world[256] = landtile(sprite(dirtSheet, 1, 2), 16, 12);
    world[257] = landtile(sprite(dirtSheet, 1, 2), 17, 12);
    world[258] = landtile(sprite(dirtSheet, 1, 2), 18, 12);
    world[259] = landtile(sprite(dirtSheet, 1, 2), 19, 12);

    world[260] = landtile(sprite(dirtSheet, 1, 2), 0, 13);
    world[261] = landtile(sprite(dirtSheet, 1, 2), 1, 13);
    world[262] = landtile(sprite(dirtSheet, 1, 2), 2, 13);
    world[263] = landtile(sprite(dirtSheet, 1, 2), 3, 13);
    world[264] = landtile(sprite(dirtSheet, 1, 2), 4, 13);
    world[265] = landtile(sprite(dirtSheet, 1, 2), 5, 13);
    world[266] = landtile(sprite(dirtSheet, 1, 2), 6, 13);
    world[267] = landtile(sprite(dirtSheet, 1, 2), 7, 13);
    world[268] = landtile(sprite(dirtSheet, 1, 2), 8, 13);
    world[269] = landtile(sprite(dirtSheet, 1, 2), 9, 13);
    world[270] = landtile(sprite(dirtSheet, 1, 2), 10, 13);
    world[271] = landtile(sprite(dirtSheet, 1, 2), 11, 13);
    world[272] = landtile(sprite(dirtSheet, 1, 2), 12, 13);
    world[273] = landtile(sprite(dirtSheet, 1, 2), 13, 13);
    world[274] = landtile(sprite(dirtSheet, 1, 2), 14, 13);
    world[275] = landtile(sprite(dirtSheet, 1, 2), 15, 13);
    world[276] = landtile(sprite(dirtSheet, 1, 2), 16, 13);
    world[277] = landtile(sprite(dirtSheet, 1, 2), 17, 13);
    world[278] = landtile(sprite(dirtSheet, 1, 2), 18, 13);
    world[279] = landtile(sprite(dirtSheet, 1, 2), 19, 13);

    world[280] = landtile(sprite(dirtSheet, 1, 2), 0, 14);
    world[281] = landtile(sprite(dirtSheet, 1, 2), 1, 14);
    world[282] = landtile(sprite(dirtSheet, 1, 2), 2, 14);
    world[283] = landtile(sprite(dirtSheet, 1, 2), 3, 14);
    world[284] = landtile(sprite(dirtSheet, 1, 2), 4, 14);
    world[285] = landtile(sprite(dirtSheet, 1, 2), 5, 14);
    world[286] = landtile(sprite(dirtSheet, 1, 2), 6, 14);
    world[287] = landtile(sprite(dirtSheet, 1, 2), 7, 14);
    world[288] = landtile(sprite(dirtSheet, 1, 2), 8, 14);
    world[289] = landtile(sprite(dirtSheet, 1, 2), 9, 14);
    world[290] = landtile(sprite(dirtSheet, 1, 2), 10, 14);
    world[291] = landtile(sprite(dirtSheet, 1, 2), 11, 14);
    world[292] = landtile(sprite(dirtSheet, 1, 2), 12, 14);
    world[293] = landtile(sprite(dirtSheet, 1, 2), 13, 14);
    world[294] = landtile(sprite(dirtSheet, 1, 2), 14, 14);
    world[295] = landtile(sprite(dirtSheet, 1, 2), 15, 14);
    world[296] = landtile(sprite(dirtSheet, 1, 2), 16, 14);
    world[297] = landtile(sprite(dirtSheet, 1, 2), 17, 14);
    world[298] = landtile(sprite(dirtSheet, 1, 2), 18, 14);
    world[299] = landtile(sprite(dirtSheet, 1, 2), 19, 14);

    for (int i = 0; i < 300; i++)
    {
        Drawable_setZ(GameObject_getDrawable(world[i]), -1.0);
    }

    return world;
}
