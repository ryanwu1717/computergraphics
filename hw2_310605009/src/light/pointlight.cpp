#include "light/pointlight.h"
#include <utility>

#include <glm/gtc/matrix_transform.hpp>

namespace graphics::light {
PointLight::PointLight(const glm::vec3& lightPosition) noexcept :
    Light(glm::vec4(lightPosition, 1), glm::vec4(0, 0, 0, 0)) {
  glm::mat4 viewProjection(1);
  // TODO (If you want to implement shadow): Set view projection matrix for light
  // Hint:
  //       1. You should use perspective projection for point light.
  //         same parameter as you camera
  //         glm::perspective (https://glm.g-truc.net/0.9.9/api/a00243.html#ga747c8cf99458663dd7ad1bb3a2f07787)
  //       2. You need to calculate the view matrix too
  //setLightSpaceMatrix(viewProjection);
  constexpr float FOV = glm::radians(45.0f);
  constexpr float zNear = 0.1f;
  constexpr float zFar = 100.0f;
  float aspectRatio = 1280 / 720;
  viewProjection = glm::perspective(FOV, aspectRatio, zNear, zFar);
  
  constexpr glm::vec3 original_front(0, 0, 1);
  constexpr glm::vec3 original_up(0, 1, 0);
  glm::mat4 viewMatrix = glm::lookAt(lightPosition, lightPosition + original_front, original_up);
  setLightSpaceMatrix(viewProjection * viewMatrix);
  //setLightSpaceMatrix(viewProjection);
}

PointLightPTR PointLight::make_unique(const glm::vec3& lightPosition) {
  return std::make_unique<PointLight>(lightPosition);
}
}  // namespace graphics::light
