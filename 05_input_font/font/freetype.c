#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <wchar.h>
#include <sys/ioctl.h>
#include <font_manager.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

static FT_Face g_tFace;
static int g_iDefaultFontSize = 12;

static int FreetypeFontInit(char *aFineName)
{
	FT_Library library;
	int err;

	err = FT_Init_FreeType(&library);     //初始化
	if(err)
	{
		printf("FT_Init_FreeType err\n");
		return -1;
	}

	err = FT_New_Face(library, aFineName, 0, &g_tFace);  //打开字体aFineName,获得g_face
	if(err)
	{
		printf("FT_New_Face err\n");
		return -1;
	}

	FT_Set_Pixel_Sizes(g_tFace, g_iDefaultFontSize, 0);

 	return 0;
}

static int FreetypeFontSize(int iFontSize)
{
	FT_Set_Pixel_Sizes(g_tFace, iFontSize, 0);
	return 0;
}

static int FreetypeGetFontBitMap(unsigned int dwCode, pFontBitMap ptFontBitMap)
{
	FT_Vector pen;
	FT_GlyphSlot slot = g_tFace->glyph;
	int err;
	
	pen.x = ptFontBitMap->iCurOriginX *64;  //原始点坐标，单位1/64像素
	pen.y = ptFontBitMap->iCurOriginY *64;

	FT_Set_Transform(g_tFace, 0, &pen);

    err = FT_Load_Char(g_tFace, dwCode, FT_LOAD_RENDER);   //加载位图
	if(err)
	{
		printf("Ft_Load_Char err\n");
		return -1;
	}

	ptFontBitMap->tRegion.iLeftUpX = slot->bitmap_left;
	ptFontBitMap->tRegion.iLeftUpY = ptFontBitMap->iCurOriginY*2 - slot->bitmap_top;  //笛卡尔坐标系转换成lcd显示坐标系，笛卡尔坐标系远点左下角，lcd是左上
	ptFontBitMap->tRegion.iHeight  = slot->bitmap.rows;
	ptFontBitMap->tRegion.iWidth   = slot->bitmap.width;
	ptFontBitMap->iNextOriginX     = ptFontBitMap->iCurOriginX + slot->advance.x / 64;  //下个字符的坐标
	ptFontBitMap->iNextOriginY	   = ptFontBitMap->iCurOriginY; //显示在同一行所以y坐标一致

	return 0;
}


static FontOpr g_tFreetypeOpr = {
	.name          = "freetype",
	.FontInit      = FreetypeFontInit,
	.SetFontSize   = FreetypeFontSize,
	.GetFontBitMap = FreetypeGetFontBitMap,
};

void FreetypeRegister(void)
{
	RegisterFont(&g_tFreetypeOpr);
}
