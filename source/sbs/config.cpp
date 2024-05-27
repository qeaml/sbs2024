#include "config.hpp"
#include <nwge/console.hpp>
#include <nwge/dialog.hpp>
#include <nwge/json.hpp>
#include <SDL2/SDL_error.h>

using namespace nwge;

namespace sbs {

static bool loadLube(Config &out, const json::Object &root);
static bool loadGravity(Config &out, const json::Object &root);
static bool loadOxy(Config &out, const json::Object &root);
static bool loadStore(Config &out, const json::Object &root);
static bool loadToilet(Config &out, const json::Object &root);
static bool loadBrick(Config &out, const json::Object &root);

bool Config::load(data::RW &file) {
  auto fileSize = file.size();
  if(fileSize <= 0) {
    dialog::error("Config", "Configuration file is invalid or empty.");
    return false;
  }

  ScratchArray<char> raw{usize(fileSize)};
  if(!file.read(raw.view())) {
    dialog::error("Config",
      "Could not read the configuration file.\n"
      "{}",
      SDL_GetError());
    return false;
  }

  auto res = json::parse(raw.view());
  if(res.error != json::OK) {
    dialog::error("Config",
      "Configuration file is not valid JSON.\n"
      "{}",
      json::errorMessage(res.error));
    return false;
  }

  if(!res.value->isObject()) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "Not an object.");
    return false;
  }
  const auto &root = res.value->object();

  if(!loadLube(*this, root)) {
    return false;
  }

  if(!loadGravity(*this, root)) {
    return false;
  }

  if(!loadOxy(*this, root)) {
    return false;
  }

  if(!loadStore(*this, root)) {
    return false;
  }

  if(!loadToilet(*this, root)) {
    return false;
  }

  if(!loadBrick(*this, root)) {
    return false;
  }

  console::note("Loaded config:");
  console::print("  Lube:");
  console::print("    Base: {}", lube.base);
  console::print("    Upgrade: {}", lube.upgrade);
  console::print("    Max Tier: {}", lube.maxTier);
  console::print("  Gravity:");
  console::print("    Base: {}", gravity.base);
  console::print("    Upgrade: {}", gravity.upgrade);
  console::print("    Threshold: {}", gravity.threshold);
  console::print("    Max Tier: {}", gravity.maxTier);
  console::print("  Toilet:");
  console::print("    X: {}", toilet.xPos);
  console::print("    Y: {}", toilet.yPos);
  console::print("    Size: {}", toilet.size);
  console::print("  Brick:");
  console::print("    Start Y: {}", brick.startY);
  console::print("    End Y: {}", brick.endY);
  console::print("    Fall Speed: {}", brick.fallSpeed);
  console::print("    Size: {}", brick.size);

  return true;
}

bool loadLube(Config &out, const json::Object &root) {
  const auto *lubeV = root.get("lube");
  if(lubeV == nullptr) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "No `lube` key.");
    return false;
  }
  if(!lubeV->isObject()) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "`lube` is not a object.");
    return false;
  }
  const auto &lubeObject = lubeV->object();

  const auto *baseV = lubeObject.get("base");
  if(baseV == nullptr) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "No `base` in key in `lube` object.");
    return false;
  }
  if(!baseV->isNumber()) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "`base` in `lube` object is not a number.");
    return false;
  }
  out.lube.base = static_cast<f32>(baseV->number());

  const auto *upgradeV = lubeObject.get("upgrade");
  if(upgradeV == nullptr) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "No `upgrade` in key in `lube` object.");
    return false;
  }
  if(!upgradeV->isNumber()) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "`upgrade` in `lube` object is not a number.");
    return false;
  }
  out.lube.upgrade = static_cast<f32>(upgradeV->number());

  const auto *maxTierV = lubeObject.get("maxTier");
  if(maxTierV == nullptr) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "No `maxTier` in key in `lube` object.");
    return false;
  }
  if(!maxTierV->isNumber()) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "`maxTier` in `lube` object is not a number.");
    return false;
  }
  out.lube.maxTier = static_cast<s16>(maxTierV->number());

  return true;
}

bool loadGravity(Config &out, const json::Object &root) {
  const auto *gravityV = root.get("gravity");
  if(gravityV == nullptr) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "No `gravity` key.");
    return false;
  }
  if(!gravityV->isObject()) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "`gravity` is not a object.");
    return false;
  }
  const auto &gravityObject = gravityV->object();

  const auto *baseV = gravityObject.get("base");
  if(baseV == nullptr) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "No `base` in key in `gravity` object.");
    return false;
  }
  if(!baseV->isNumber()) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "`base` in `gravity` object is not a number.");
    return false;
  }
  out.gravity.base = static_cast<f32>(baseV->number());

  const auto *upgradeV = gravityObject.get("upgrade");
  if(upgradeV == nullptr) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "No `upgrade` in key in `gravity` object.");
    return false;
  }
  if(!upgradeV->isNumber()) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "`upgrade` in `gravity` object is not a number.");
    return false;
  }
  out.gravity.upgrade = static_cast<f32>(upgradeV->number());

  const auto *thresholdV = gravityObject.get("threshold");
  if(thresholdV == nullptr) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "No `threshold` in key in `gravity` object.");
    return false;
  }
  if(!thresholdV->isNumber()) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "`threshold` in `gravity` object is not a number.");
    return false;
  }
  out.gravity.threshold = static_cast<f32>(thresholdV->number());

  const auto *maxTierV = gravityObject.get("maxTier");
  if(maxTierV == nullptr) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "No `maxTier` in key in `gravity` object.");
    return false;
  }
  if(!maxTierV->isNumber()) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "`maxTier` in `gravity` object is not a number.");
    return false;
  }
  out.gravity.maxTier = static_cast<s16>(maxTierV->number());

  return true;
}

bool loadOxy(Config &out, const json::Object &root) {
  const auto *oxyV = root.get("oxy");
  if(oxyV == nullptr) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "No `oxy` key.");
    return false;
  }
  if(!oxyV->isObject()) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "`oxy` is not a object.");
    return false;
  }
  const auto &oxyObject = oxyV->object();

  const auto *regenV = oxyObject.get("regen");
  if(regenV == nullptr) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "No `regen` in key in `oxy` object.");
    return false;
  }
  if(!regenV->isNumber()) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "`regen` in `oxy` object is not a number.");
    return false;
  }
  out.oxy.regen = static_cast<f32>(regenV->number());

  const auto *drainV = oxyObject.get("drain");
  if(drainV == nullptr) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "No `drain` in key in `oxy` object.");
    return false;
  }
  if(!drainV->isNumber()) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "`drain` in `oxy` object is not a number.");
    return false;
  }
  out.oxy.drain = static_cast<f32>(drainV->number());

  const auto *minV = oxyObject.get("min");
  if(minV == nullptr) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "No `min` in key in `oxy` object.");
    return false;
  }
  if(!minV->isNumber()) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "`min` in `oxy` object is not a number.");
    return false;
  }
  out.oxy.min = static_cast<f32>(minV->number());

  return true;
}

bool loadStore(Config &out, const json::Object &root) {
  const auto *storeV = root.get("store");
  if(storeV == nullptr) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "No `store` key.");
    return false;
  }
  if(!storeV->isArray()) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "`store` is not an array.");
    return false;
  }
  const auto &storeArray = storeV->array();

  out.store = {storeArray.size()};
  for(usize i = 0; i < out.store.size(); ++i) {
    auto &item = out.store[i];
    const auto &itemV = storeArray[i];
    if(!itemV.isObject()) {
      dialog::error("Config",
        "Configuration file is invalid.\n"
        "`store` element {} is not an object.",
        i);
      return false;
    }
    const auto &itemObject = itemV.object();

    const auto *nameV = itemObject.get("name");
    if(nameV == nullptr || !nameV->isString()) {
      dialog::error("Config",
        "Configuration file is invalid.\n"
        "`name` of `store` element {} is not a string.",
        i);
      return false;
    }
    item.name = nameV->string();

    const auto *descV = itemObject.get("desc");
    if(descV == nullptr || !descV->isString()) {
      dialog::error("Config",
        "Configuration file is invalid.\n"
        "`desc` of `store` element {} is not a string.",
        i);
      return false;
    }
    item.desc = descV->string();

    const auto *priceV = itemObject.get("price");
    if(priceV == nullptr || !priceV->isNumber()) {
      dialog::error("Config",
        "Configuration file is invalid.\n"
        "`price` of `store` element {} is not a Number.",
        i);
      return false;
    }
    item.price = static_cast<s16>(priceV->number());

    const auto *iconV = itemObject.get("icon");
    if(iconV == nullptr || !iconV->isNumber()) {
      dialog::error("Config",
        "Configuration file is invalid.\n"
        "`icon` of `store` element {} is not a Number.",
        i);
      return false;
    }
    item.icon = static_cast<s16>(iconV->number());

    const auto *lubeTierV = itemObject.get("lubeTier");
    if(lubeTierV != nullptr) {
      item.kind = StoreItem::Lube;
      if(!lubeTierV->isNumber()) {
        dialog::error("Config",
          "Configuration file is invalid.\n"
          "`lubeTier` of `store` element {} is not a Number.",
          i);
        return false;
      }
      item.argument = static_cast<s16>(lubeTierV->number());
      continue;
    }
    const auto *gravityTierV = itemObject.get("gravityTier");
    if(gravityTierV != nullptr) {
      item.kind = StoreItem::Gravity;
      if(!gravityTierV->isNumber()) {
        dialog::error("Config",
          "Configuration file is invalid.\n"
          "`gravityTier` of `store` element {} is not a Number.",
          i);
        return false;
      }
      item.argument = static_cast<s16>(gravityTierV->number());
      continue;
    }
    const auto *endGameV = itemObject.get("endGame");
    if(endGameV != nullptr) {
      item.kind = StoreItem::EndGame;
      continue;
    }
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "`store` element {} does not define `lubeTier`, `gravityTier` or `endGame`.",
      i);
    return false;
  }
  return true;
}

bool loadToilet(Config &out, const json::Object &root) {
  const auto *toiletV = root.get("toilet");
  if(toiletV == nullptr) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "No `toilet` key.");
    return false;
  }
  if(!toiletV->isObject()) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "`toilet` is not a object.");
    return false;
  }
  const auto &toiletObject = toiletV->object();

  const auto *xPosV = toiletObject.get("xPos");
  if(xPosV == nullptr) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "No `xPos` in key in `toilet` object.");
    return false;
  }
  if(!xPosV->isNumber()) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "`xPos` in `toilet` object is not a number.");
    return false;
  }
  out.toilet.xPos = static_cast<f32>(xPosV->number());

  const auto *yPosV = toiletObject.get("yPos");
  if(yPosV == nullptr) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "No `yPos` in key in `toilet` object.");
    return false;
  }
  if(!yPosV->isNumber()) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "`yPos` in `toilet` object is not a number.");
    return false;
  }
  out.toilet.yPos = static_cast<f32>(yPosV->number());

  const auto *sizeV = toiletObject.get("size");
  if(sizeV == nullptr) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "No `size` in key in `toilet` object.");
    return false;
  }
  if(!sizeV->isNumber()) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "`size` in `toilet` object is not a number.");
    return false;
  }
  out.toilet.size = static_cast<f32>(sizeV->number());

  return true;
}

bool loadBrick(Config &out, const json::Object &root) {
  const auto *brickV = root.get("brick");
  if(brickV == nullptr) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "No `brick` key.");
    return false;
  }
  if(!brickV->isObject()) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "`brick` is not a object.");
    return false;
  }
  const auto &brickObject = brickV->object();

  const auto *xPosV = brickObject.get("xPos");
  if(xPosV == nullptr) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "No `xPos` in key in `brick` object.");
    return false;
  }
  if(!xPosV->isNumber()) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "`xPos` in `brick` object is not a number.");
    return false;
  }
  out.brick.xPos = static_cast<f32>(xPosV->number());

  const auto *startYV = brickObject.get("startY");
  if(startYV == nullptr) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "No `startY` in key in `brick` object.");
    return false;
  }
  if(!startYV->isNumber()) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "`startY` in `brick` object is not a number.");
    return false;
  }
  out.brick.startY = static_cast<f32>(startYV->number());

  const auto *endYV = brickObject.get("endY");
  if(endYV == nullptr) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "No `endY` in key in `brick` object.");
    return false;
  }
  if(!endYV->isNumber()) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "`endY` in `brick` object is not a number.");
    return false;
  }
  out.brick.endY = static_cast<f32>(endYV->number());

  const auto *fallSpeedV = brickObject.get("fallSpeed");
  if(fallSpeedV == nullptr) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "No `fallSpeed` in key in `brick` object.");
    return false;
  }
  if(!fallSpeedV->isNumber()) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "`fallSpeed` in `brick` object is not a number.");
    return false;
  }
  out.brick.fallSpeed = static_cast<f32>(fallSpeedV->number());

  const auto *sizeV = brickObject.get("size");
  if(sizeV == nullptr) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "No `size` in key in `brick` object.");
    return false;
  }
  if(!sizeV->isNumber()) {
    dialog::error("Config",
      "Configuration file is invalid.\n"
      "`size` in `brick` object is not a number.");
    return false;
  }
  out.brick.size = static_cast<f32>(sizeV->number());

  return true;
}

} // namespace sbs