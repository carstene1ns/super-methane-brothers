#include "precomp.h"
#include "global.h"
#include "gfxdef.h"
#include "names.h"

struct sprite_struct {
	std::string name;
	FIBITMAP* image;
};

void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message) {
	printf("\n*** ");
	if(fif != FIF_UNKNOWN) {
		printf("%s Format\n", FreeImage_GetFormatFromFIF(fif));
	}
	printf(message);
	printf(" ***\n");
}


bool savePng(FIBITMAP *img, std::string name) {
	if (!img) return false;

	if(!FreeImage_Save(FIF_PNG, img, name.c_str(), PNG_Z_BEST_COMPRESSION)) {
		printf("failed to save image %s\n", name.c_str());
		return false;
	}

	return true;
}

bool saveT3s(std::vector<sprite_struct> &pngs, std::string name) {
	std::ofstream t3s;
	t3s.open(name);

	if(!t3s.good()) return false;

	// create atlas description
	t3s << "--atlas\n";
	t3s << "--format rgba\n";
	t3s << "--compress lz11\n";
	t3s << "--border transparent\n";

	// add images
	for(const auto& png : pngs)
		t3s << png.name << "\n";

	t3s.close();

	return true;
}

FIBITMAP *createPlaceholder() {
	int w = 16, h = 16;
	FIBITMAP *result = FreeImage_Allocate(w, h, 32);
	BYTE *bits = (BYTE*)FreeImage_GetBits(result);

	for(int y = 0; y < h; y++) {
		BYTE *pixel = (BYTE*)bits;
		for(int x = 0; x < w; x++) {
			pixel[FI_RGBA_RED] = 255;
			// red cross
			if (x == y || x == w - y - 1) {
				pixel[FI_RGBA_GREEN] = 0;
				pixel[FI_RGBA_BLUE] = 0;
			} else {
				pixel[FI_RGBA_GREEN] = 255;
				pixel[FI_RGBA_BLUE] = 255;
			}
			pixel[FI_RGBA_ALPHA] = 255;
			pixel += 4;
		}
		// next line
		bits += w*4;
	}

	return result;
}

int ProcessBlocks(std::vector<sprite_struct> &blocks, MCOORDS &offset,
	std::string &filename, std::vector<FIBITMAP*> &spritesheets) {
	int block_count = 0;
	int h = FreeImage_GetHeight(spritesheets[offset.texture_number]);
	int w = FreeImage_GetWidth(spritesheets[offset.texture_number]);

	// find end position
	int block_end_x = w;
	int block_end_y = h;
	int tex_num = offset.texture_number;

	auto FindBlockEnd = [&block_end_x, &block_end_y](int x, int y, int tex_num) {
		for(int i = 0; i < SPR_SIZE; i++) {
			MCOORDS o = MainOffsets[i];

			// only look on current sheet
			if (o.texture_number != tex_num) continue;

			// save minimum y
			if(o.texture_ypos > y && o.texture_ypos < block_end_y)
				block_end_y = o.texture_ypos;
		}
		for(int i = 0; i < SPR_SIZE; i++) {
			MCOORDS o = MainOffsets[i];

			// only look on current sheet
			if (o.texture_number != tex_num) continue;

			// only look at y below end mark
			if (o.texture_ypos != block_end_y) continue;

			// save minimum x
			if(o.texture_xpos < block_end_x)
				block_end_x = o.texture_xpos;
		}
	};

	auto MakeBlock = [&](int x, int y) {
		block_count++;
		//printf("block: x = %d -> %d, y = %d -> %d\n", x, x+16, y, y+16);

		// create view
		FIBITMAP *view = FreeImage_CreateView(spritesheets[tex_num], x, y, x+16, y+16);

		sprite_struct s = {"png/" + filename + "_" + std::to_string(block_count) + ".png", view};
		blocks.emplace_back(s);
	};

	FindBlockEnd(offset.texture_xpos, offset.texture_ypos, tex_num);
	if(block_end_x == w && block_end_y == h) {
		// all blocks
		for(int y = offset.texture_ypos; y < h; y += 16)
			for(int x = offset.texture_xpos; x < w; x += 16)
				MakeBlock(x, y);

		printf("reached end of spritesheet, adding next.\n");
		FindBlockEnd(0, 0, ++tex_num);
	}

	if(block_end_x < w || block_end_y < h) {
		printf("next texture starts at %d, %d\n", block_end_x, block_end_y);

		for (int y = offset.texture_ypos; y <= block_end_y; y += 16) {
			// first line
			int start_x = (y == offset.texture_ypos)?offset.texture_xpos:0;
			// last line
			int end_x = (y == block_end_y)?block_end_x:w;

			for(int x = start_x; x < end_x; x += 16) {
				MakeBlock(x, y);
			}
		}
	}

	return block_count;
}

int main(int argc, char const *argv[]) {
	FreeImage_Initialise(true);
	FreeImage_SetOutputMessage(FreeImageErrorHandler);

	std::vector<FIBITMAP*> spritesheets(5);
	std::string filename;
	std::vector<sprite_struct> sprites;
	std::vector<sprite_struct> blocks;

	// load all spritesheets
	for (int i = 0; i < 5; i++) {
		filename = "../../../resources/graphics/page_0" + std::to_string(i+1) + ".png";
		spritesheets[i] = FreeImage_Load(FIF_PNG, filename.c_str(), PNG_DEFAULT);
		if(!spritesheets[i]) {
			printf("failed to load spritesheet %s\n", filename.c_str());
			exit(1);
		}
	}

	// create output folder
    fs::create_directory("png");

	// process list
	for(int i = 0; i < SPR_SIZE; i++) {
		MCOORDS offset = MainOffsets[i];
		filename.assign(sprite_names[i]);

		FIBITMAP* sprite = nullptr;
		if (offset.height == 0) {
			printf("writing placeholder for block data %s\n", filename.c_str());

			// create placeholder image to have correct sprite numbers
			sprite = createPlaceholder();

			// big question is how many blocks?
			int block_count = ProcessBlocks(blocks, offset, filename, spritesheets);
			printf("processed %d blocks!\n", block_count);
		} else {
			// create view
			sprite = FreeImage_CreateView(
				spritesheets[offset.texture_number],      // 
				offset.texture_xpos, offset.texture_ypos, // left, top
				offset.texture_xpos + offset.width,       // right
				offset.texture_ypos + offset.height);     // bottom
		}

		if (sprite) {
			sprite_struct s = {"png/" + filename + ".png", sprite};
			sprites.emplace_back(s);
		}
	}

	// save t3s
	saveT3s(sprites, "sprites.t3s");
	saveT3s(blocks, "blocks.t3s");

	// merge sprites and blocks
	sprites.reserve(blocks.size() + sprites.size());
	sprites.insert(sprites.end(), blocks.begin(), blocks.end());

	// saves sprite and block png
	for(const auto &img : sprites) {
		savePng(img.image, img.name);

		// destroy view/placeholder
		FreeImage_Unload(img.image);
	}

	// unload spritesheets
	for (int i = 0; i < 5; i++)
		FreeImage_Unload(spritesheets[i]);

	FreeImage_DeInitialise();
	return 0;
}
