#include <metal_stdlib>
using namespace metal;

struct Vertex {
    float4 position [[position]];
    float4 color;
};

vertex Vertex vertex_main(uint vertex_id [[vertex_id]]) {
    float3 positions[6] = {
        float3(0.0, 0.5, 0.0),
        float3(-0.5, -0.5, 0.0),
        float3(0.5, -0.5, 0.0),
        float3(-0.5, 0.5, 0.0),
        float3(0.5, 0.5, 0.0),
        float3(0.0, -0.5, 0.0)
    };
    float4 colors[6] = {
        float4(1.0, 0.0, 0.0, 1.0),
        float4(0.0, 1.0, 0.0, 1.0),
        float4(0.0, 0.0, 1.0, 1.0),
        float4(1.0, 1.0, 0.0, 1.0),
        float4(1.0, 0.0, 1.0, 1.0),
        float4(0.0, 1.0, 1.0, 1.0)
    };

    Vertex out;
    out.position = float4(positions[vertex_id], 1.0);
    out.color = colors[vertex_id];
    return out;
}
