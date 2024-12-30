#include <metal_stdlib>
using namespace metal;

struct Vertex {
    float4 position [[position]];
    float4 color;
};

fragment float4 fragment_main(Vertex in [[stage_in]]) {
    return in.color;
}
