#include <gb/gb.h>


UINT8 solid[] = {
	0xFFU, 0xFFU,
	0xFFU, 0xFFU,
	0xFFU, 0xFFU,
	0xFFU, 0xFFU,
	0xFFU, 0xFFU,
	0xFFU, 0xFFU,
	0xFFU, 0xFFU,
	0xFFU, 0xFFU
};

UINT8 half[] = {
	0x00U, 0xFFU,
	0x00U, 0xFFU,
	0x00U, 0xFFU,
	0x00U, 0xFFU,
	0x00U, 0xFFU,
	0x00U, 0xFFU,
	0x00U, 0xFFU,
	0x00U, 0xFFU
};

UINT8 binary[] =
{
	0x00U, 0x00U,
	0x3CU, 0x3CU,
	0x42U, 0x42U,
	0x42U, 0x42U,
	0x42U, 0x42U,
	0x42U, 0x42U,
	0x3CU, 0x3CU,
	0x00U, 0x00U,

	0x00U, 0x00U,
	0x18U, 0x18U,
	0x08U, 0x08U,
	0x08U, 0x08U,
	0x08U, 0x08U,
	0x08U, 0x08U,
	0x08U, 0x08U,
	0x00U, 0x00U
};


// grids should be 16 * 16
UINT8 gridData[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
	0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
	0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
	0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


typedef union {
	struct {
	UINT8 fracp;
	UINT8 intp;
	
	} U8;
	UINT16 U16;

} UFIX16;

typedef struct {
	UFIX16 x;
	UFIX16 y;

} UVEC2;

typedef struct {
	INT16 x;
	INT16 y;

} VEC2;

typedef struct {
	UVEC2 pos;
	UINT8 width;
	UINT8 height;

} RECTANGLE;

typedef struct {
	RECTANGLE shape;
	VEC2 velocity;

} DYNOBJ;


void BkgClear(
	UINT8 tileIndex);

void BkgSetToGrid(
	volatile UINT8 *grid);

void DYNOBJSet(
	volatile DYNOBJ *obj,
	UINT8 x,
	UINT8 y,
	UINT8 width,
	UINT8 height);

void Move(
	volatile DYNOBJ *obj,
	volatile UINT8 *grid);

void PrintUFix16(
	UINT8 line,
	volatile UFIX16 *variable);


DYNOBJ player;


void VBLANK()
{
	Move(&player, gridData);
	PrintUFix16(0, &player.velocity.x);
	PrintUFix16(1, &player.velocity.y);

	move_sprite(0, player.shape.pos.x.U8.intp + 1, player.shape.pos.y.U8.intp + 9);
}

void main()
{
	// current move implementation assumes a maximim size of 8x8
	DYNOBJSet(&player, 16, 16, 8, 8);


	set_bkg_data(1, 2, half);
	BkgClear(0);
	BkgSetToGrid(gridData);

	set_bkg_data(2, 2, binary);

	set_sprite_data(0, 1, solid);
	set_sprite_tile(0, 0);

	SPRITES_8x8;
	SHOW_SPRITES;
	SHOW_BKG;
	HIDE_WIN;

	add_VBL(VBLANK);
	set_interrupts(VBL_IFLAG);
	enable_interrupts();
}


void BkgClear(
	UINT8 tileIndex)
{
	UINT8 x;
	UINT8 y;

	for (x = 0; x < 32; x++)
	{
		for (y = 0; y < 32; y++)
		{
			set_bkg_tiles(x, y, 1, 1, &tileIndex);
		}
	}
}
void BkgSetToGrid(
	volatile UINT8 *grid)
{
	set_bkg_tiles(0, 0, 16, 16, grid);
}

void DYNOBJSet(
	volatile DYNOBJ *obj,
	UINT8 x,
	UINT8 y,
	UINT8 width,
	UINT8 height)
{
	obj->shape.pos.x.U8.intp = x;
	obj->shape.pos.x.U8.fracp = 0;
	obj->shape.pos.y.U8.intp = y;
	obj->shape.pos.y.U8.fracp = 0;
	obj->shape.width = width;
	obj->shape.height = height;
	obj->velocity.x = 0;
	obj->velocity.y = 0;
}

void Move(
	volatile DYNOBJ *obj,
	volatile UINT8 *grid)
{
	const INT16 acceleration = 0x0001;

	UINT8 input;

	UINT8 yindex;
	UINT8 yindexAlt;
	UINT8 xindex;
	UINT8 xindexAlt;

	input = joypad();


	if (input & J_LEFT)
	{
		obj->velocity.x = obj->velocity.x - acceleration;
	}

	if (input & J_RIGHT)
	{
		obj->velocity.x = obj->velocity.x + acceleration;
	}

	if (input & J_UP)
	{
		obj->velocity.y = obj->velocity.y - acceleration;
	}

	if (input & J_DOWN)
	{
		obj->velocity.y = obj->velocity.y + acceleration;
	}

	// clamp for max velocities

	obj->velocity.y = obj->velocity.y + 0x0008;	// gravity

	
	obj->shape.pos.x.U16 = obj->shape.pos.x.U16 + obj->velocity.x;

	yindex = (obj->shape.pos.y.U8.intp >> 3) << 4;
	yindexAlt = ((obj->shape.pos.y.U8.intp - 7) >> 3) << 4;

	if (obj->velocity.x < 0)
	{
		xindex = (obj->shape.pos.x.U8.intp - 7) >> 3;

		if (grid[xindex + yindex] || grid[xindex + yindexAlt])
		{
			obj->shape.pos.x.U8.intp = (xindex << 3) + 15;
			obj->shape.pos.x.U8.fracp = 0;
			obj->velocity.x = 0;
		}
	}
	else if (obj->velocity.x > 0)
	{
		xindex = obj->shape.pos.x.U8.intp >> 3;

		if (grid[xindex + yindex] || grid[xindex + yindexAlt])
		{
			obj->shape.pos.x.U8.intp = (xindex << 3) - 1;
			obj->shape.pos.x.U8.fracp = 0xFF;
			obj->velocity.x = 0;
		}
	}


	obj->shape.pos.y.U16 = obj->shape.pos.y.U16 + obj->velocity.y;

	xindex = obj->shape.pos.x.U8.intp >> 3;
	xindexAlt = (obj->shape.pos.x.U8.intp - 7) >> 3;

	if (obj->velocity.y < 0)
	{
		yindex = ((obj->shape.pos.y.U8.intp - 7) >> 3) << 4;

		if (grid[xindex + yindex] || grid[xindexAlt + yindex])
		{
			obj->shape.pos.y.U8.intp = (obj->shape.pos.y.U8.intp & 0xF8) + 7;
			obj->shape.pos.y.U8.fracp = 0;
			obj->velocity.y = 0;
		}
	}
	else if (obj->velocity.y > 0)
	{
		yindex = (obj->shape.pos.y.U8.intp >> 3) << 4;

		if (grid[xindex + yindex] || grid[xindexAlt + yindex])
		{
			obj->shape.pos.y.U8.intp = (obj->shape.pos.y.U8.intp & 0xF8) - 1;
			obj->shape.pos.y.U8.fracp = 0xFF;
			obj->velocity.y = 0;
		}
	}
}

void PrintUFix16(
	UINT8 line,
	volatile UFIX16 *variable)
{
	UINT16 i;
	UINT8 value;
	for (i = 0; i < 16; i++)
	{
		value = ((variable->U16) & ((UINT16)1 << ((UINT16)15 - i))) ? 3 : 2;
		set_bkg_tiles(i, line, 1, 1, &value);
	}
}