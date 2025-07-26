#include <metal_stdlib>
using namespace metal;

struct VertexIn {
  packed_float2 position;
  packed_float2 texCoords;
};

struct VertexOut {
  float4 position [[position]];
  float2 texCoords;
};

vertex VertexOut vertexShader(
  const device VertexIn* vertices [[buffer(0)]],
  unsigned int vertexID [[vertex_id]]) {
  
  VertexOut out;
  out.position = float4(vertices[vertexID].position, 0.0, 1.0);
  out.texCoords = vertices[vertexID].texCoords;
  return out;
}

fragment float4 fragmentShader(
  VertexOut in [[stage_in]],
  texture2d<float> renderTexture [[texture(0)]]) {
  
  constexpr sampler s(address::clamp_to_edge, filter::linear);
  return renderTexture.sample(s, in.texCoords);
}
