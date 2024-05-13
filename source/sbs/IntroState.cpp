#include "states.hpp"
#include <nwge/data/bundle.hpp>
#include <nwge/render/aspectRatio.hpp>
#include <nwge/render/draw.hpp>
#include <nwge/render/gl/Texture.hpp>
#include <nwge/render/window.hpp>

using namespace nwge;

namespace sbs {

class IntroState: public State {
private:
  data::Bundle mBundle;
  render::gl::Texture mLogo;

  f32 mFadeIn = 0.0f;
  f32 mLinger = 0.0f;
  f32 mFadeOut = 0.0f;
  static constexpr f32
    cFadeInDur = 2.0f,
    cLingerDur = 3.0f,
    cFadeOutDur = 2.0f;

  render::AspectRatio m1x1{1, 1};
  static constexpr f32
    cLogoSide = 0.7f,
    cLogoOff = (1.0f - cLogoSide) / 2.0f,
    cLogoZ = 0.5f;
  static constexpr glm::vec3 cLogoPos{cLogoOff, cLogoOff, cLogoZ};
  static constexpr glm::vec2 cLogoSize{cLogoSide, cLogoSide};

public:
  bool preload() override {
    mBundle
      .load({"sbs.bndl"})
      .nqTexture("logo1.png", mLogo);
    return true;
  }

  bool tick(f32 delta) override {
    if(mFadeIn < cFadeInDur) {
      mFadeIn += delta;
      return true;
    }
    if(mLinger < cLingerDur) {
      mLinger += delta;
      return true;
    }
    if(mFadeOut < cFadeOutDur) {
      mFadeOut += delta;
      return true;
    }
    swapStatePtr(getMenuState());
    return true;
  }

  void render() const override {
    render::clear({0, 0, 0});
    if(mFadeIn < cFadeInDur) {
      render::color({1, 1, 1, mFadeIn/cFadeInDur});
    } else if(mLinger < cLingerDur) {
      render::color();
    } else if(mFadeOut < cFadeOutDur) {
      render::color({1, 1, 1, 1.0f - mFadeOut/cFadeOutDur});
    }
    render::rect(m1x1.pos(cLogoPos), m1x1.size(cLogoSize), mLogo);
  }
};

State *getIntroState() {
  return new IntroState;
}

} // namespace sbs
