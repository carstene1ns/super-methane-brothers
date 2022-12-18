
#pragma once

/// \brief ClanMikMod initialization.
class SetupMikMod
{
public:
	/// \brief Initializes clanMikMod.
	///
	/// Constructing a SetupMikMod object is equalent to calling SetupMikMod::init().
	///
	/// When the SetupMikMod instance is destroyed, SetupMikMod::deinit() is called.
	SetupMikMod(bool register_resources_only = false);
	~SetupMikMod();

	/// \brief Call this function in init_modules() to use clanMikMod.
	static void init(bool register_resources_only = false);

	/// \brief Call this function in deinit_modules().
	static void deinit();
};
