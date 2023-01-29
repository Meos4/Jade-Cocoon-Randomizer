#include "Shop.hpp"

#include "Backend/File.hpp"
#include "Backend/Item.hpp"
#include "Backend/Mips.hpp"
#include "Backend/Random.hpp"

#include <algorithm>
#include <array>
#include <unordered_map>
#include <utility>

static constexpr auto availableShop{ 5u };

using Shop_t = u16;

enum : Shop_t
{
	SHOP_BEETLE = 1 << 0,
	SHOP_DRAGONFLY = 1 << 1,
	SHOP_SPIDER = 1 << 2,
	SHOP_NETHERWORLD = 1 << 3,
	SHOP_ETERNAL_CORRIDOR = 1 << 4,
	SHOP_DEBUG = 1 << 15
};

template <Item::Category_t Category, std::size_t Size>
static void monoShopRandomizer(std::array<Shop_t, Size>* items, Shop_t shop, s32 size)
{
	static_assert(Category <= Item::CATEGORY_OTHER, "Invalid Shop Category");

	const std::unordered_map<Shop_t, Chapter_t> chapters
	{
		{ SHOP_BEETLE, CHAPTER_BEETLE }, { SHOP_DRAGONFLY, CHAPTER_DRAGONFLY },
		{ SHOP_SPIDER, CHAPTER_SPIDER }, { SHOP_NETHERWORLD, CHAPTER_NETHERWORLD },
		{ SHOP_ETERNAL_CORRIDOR, CHAPTER_ETERNAL_CORRIDOR }
	};

	auto availableItems{ Item::availableItemsChapter(chapters.at(shop))[Category] };

	if constexpr (Category == Item::CATEGORY_ITEM)
	{
		static constexpr std::array<Item_t, 14> invalidItems
		{
			ITEM_GREAT_WALNUT, ITEM_CHESTNUT_OIL, ITEM_SHISHIUDO_OIL, ITEM_KUKUMIRA_OIL,
			ITEM_BLETILLA_OIL, ITEM_ICHISHI_OIL, ITEM_CLOUD_SILK, ITEM_THUNDER_SILK,
			ITEM_LILY_SILK, ITEM_PEARL_SILK, ITEM_FIRST_SNOW_SILK, ITEM_ANGELWING_SILK,
			ITEM_MOONLIGHT_SILK, ITEM_SKELETON_KEY
		};

		auto isAnInvalidItem = [](auto item)
		{
			for (const auto& invalidItem : invalidItems)
			{
				if (item == invalidItem)
				{
					return true;
				}
			}
			return false;
		};

		const auto [begin, end] { std::ranges::remove_if(availableItems, isAnInvalidItem) };
		availableItems.erase(begin, end);
	}

	for (s32 i{}; i < size; ++i)
	{
		const auto rngItem{ Random::get().generate(availableItems.size() - 1) };
		(*items)[availableItems.at(rngItem)] |= shop;
		availableItems.erase(availableItems.begin() + rngItem);
	}
}

template <Item::Category_t Category>
static auto shopRandomizer(const std::array<s32, availableShop>& sizes)
{
	static constexpr std::array<Item_t, 4> itemsCount
	{
		ITEM_COUNT, WEAPON_COUNT, ARMOR_COUNT, OTHER_COUNT
	};

	std::array<Shop_t, itemsCount[Category]> items{};

	for (u32 i{}; i < availableShop; ++i)
	{
		monoShopRandomizer<Category>(&items, 1 << i, sizes[i]);
	}

	return items;
}

Shop::Shop(std::shared_ptr<Game> game, std::shared_ptr<SharedData> sharedData)
	: m_game(std::move(game)), m_sharedData(std::move(sharedData))
{
}

void Shop::setWeapon() const
{
	m_game->file(File::OVER_WPNSHOP_BIN)->write(
		m_game->offset().file.over_wpnshop_bin.items + 0x58, shopRandomizer<Item::CATEGORY_WEAPON>({ 3, 4, 6, 8, 17 }));
}

void Shop::setArmor() const
{
	m_game->file(File::OVER_WPNSHOP_BIN)->write(
		m_game->offset().file.over_wpnshop_bin.items + 0x88, shopRandomizer<Item::CATEGORY_ARMOR>({ 2, 4, 6, 4, 13 }));
}

void Shop::setOther() const
{
	m_game->file(File::OVER_WPNSHOP_BIN)->write(
		m_game->offset().file.over_wpnshop_bin.items + 0xA8, shopRandomizer<Item::CATEGORY_OTHER>({ 1, 2, 3, 7, 16 }));
}

void Shop::setItem() const
{
	m_game->file(File::OVER_WPNSHOP_BIN)->write(
		m_game->offset().file.over_wpnshop_bin.items, shopRandomizer<Item::CATEGORY_ITEM>({ 3, 5, 7, 27, 27 }));
}

void Shop::setEternalCorridorUnlockAll() const
{
	const auto over_wpnshop_bin{ m_game->file(File::OVER_WPNSHOP_BIN) };

	auto writeShop = [&over_wpnshop_bin]<Item_t Size, Item::Category_t Category>(u32 offset)
	{
		auto items{ over_wpnshop_bin->read<std::array<Shop_t, Size>>(offset) };
		monoShopRandomizer<Category>(&items, SHOP_ETERNAL_CORRIDOR, Category == Item::CATEGORY_ITEM ? Size - 14 : Size);
		over_wpnshop_bin->write(offset, items);
	};

	writeShop.operator()<WEAPON_COUNT, Item::CATEGORY_WEAPON>(m_game->offset().file.over_wpnshop_bin.items + 0x58);
	writeShop.operator()<ARMOR_COUNT, Item::CATEGORY_ARMOR>(m_game->offset().file.over_wpnshop_bin.items + 0x88);
	writeShop.operator()<OTHER_COUNT, Item::CATEGORY_OTHER>(m_game->offset().file.over_wpnshop_bin.items + 0xA8);
	writeShop.operator()<ITEM_COUNT, Item::CATEGORY_ITEM>(m_game->offset().file.over_wpnshop_bin.items);

	over_wpnshop_bin->write
	(
		m_game->offset().file.over_wpnshop_bin.initEternalCorridorShopFn + 0x108,
		Mips::li(Mips::Register::a1, WEAPON_COUNT)
	);

	over_wpnshop_bin->write
	(
		m_game->offset().file.over_wpnshop_bin.initEternalCorridorShopFn + 0x118,
		Mips::li(Mips::Register::a1, ARMOR_COUNT)
	);

	over_wpnshop_bin->write
	(
		m_game->offset().file.over_wpnshop_bin.initEternalCorridorShopFn + 0x130,
		Mips::li(Mips::Register::a1, OTHER_COUNT)
	);

	over_wpnshop_bin->write
	(
		m_game->offset().file.over_wpnshop_bin.initEternalCorridorShopFn + 0x148,
		Mips::li(Mips::Register::a1, ITEM_COUNT)
	);
}