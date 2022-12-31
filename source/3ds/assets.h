#pragma once

#include "blocks_t3x.h"
#include "sprites_t3x.h"

#include "boss_mod.h"
#include "complete_mod.h"
#include "title_mod.h"
#include "tune1_mod.h"
#include "tune2_mod.h"

#include "blow_raw.h"
#include "bowling_raw.h"
#include "candle_raw.h"
#include "card_raw.h"
#include "car_raw.h"
#include "chicken_raw.h"
#include "cookie_raw.h"
#include "crying_raw.h"
#include "day_raw.h"
#include "die2_raw.h"
#include "duck_raw.h"
#include "feather_raw.h"
#include "finlev1_raw.h"
#include "hurry_raw.h"
#include "marble_raw.h"
#include "mask_raw.h"
#include "moon_raw.h"
#include "oil_raw.h"
#include "pickup1_raw.h"
#include "pstar_raw.h"
#include "redstar_raw.h"
#include "spiningtop_raw.h"
#include "spit_raw.h"
#include "splat_raw.h"
#include "tap_raw.h"
#include "train_raw.h"
#include "tribble_raw.h"
#include "turbo_raw.h"
#include "twinkle_raw.h"
#include "wings_raw.h"
#include "wpotion_raw.h"
#include "xylo_raw.h"

const uint8_t *assets[] = {
	tune1_mod, tune2_mod, boss_mod, complete_mod, title_mod,

	car_raw, train_raw, duck_raw, pickup1_raw, tribble_raw, hurry_raw, day_raw, crying_raw,
	die2_raw, spit_raw, splat_raw, blow_raw, twinkle_raw, finlev1_raw, pstar_raw, xylo_raw,
	card_raw, bowling_raw, candle_raw, marble_raw, tap_raw, oil_raw, spiningtop_raw,
	wings_raw, moon_raw, mask_raw, redstar_raw, turbo_raw, chicken_raw, feather_raw,
	wpotion_raw, cookie_raw
};

const size_t asset_sizes[] = {
	tune1_mod_size, tune2_mod_size, boss_mod_size, complete_mod_size, title_mod_size,

	car_raw_size, train_raw_size, duck_raw_size, pickup1_raw_size, tribble_raw_size,
	hurry_raw_size, day_raw_size, crying_raw_size, die2_raw_size, spit_raw_size,
	splat_raw_size, blow_raw_size, twinkle_raw_size, finlev1_raw_size, pstar_raw_size,
	xylo_raw_size, card_raw_size, bowling_raw_size, candle_raw_size, marble_raw_size,
	tap_raw_size, oil_raw_size, spiningtop_raw_size, wings_raw_size, moon_raw_size,
	mask_raw_size, redstar_raw_size, turbo_raw_size, chicken_raw_size, feather_raw_size,
	wpotion_raw_size, cookie_raw_size
};

#define ASSET_RAWSTART 5
