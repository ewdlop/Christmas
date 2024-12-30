struct VertexOutput {
    @builtin(position) position : vec4<f32>;
    @location(0) color : vec4<f32>;
};

@vertex
fn main(@builtin(vertex_index) vertexIndex : u32) -> VertexOutput {
    var positions = array<vec3<f32>, 6>(
        vec3<f32>(0.0, 0.5, 0.0),
        vec3<f32>(-0.5, -0.5, 0.0),
        vec3<f32>(0.5, -0.5, 0.0),
        vec3<f32>(-0.5, 0.5, 0.0),
        vec3<f32>(0.5, 0.5, 0.0),
        vec3<f32>(0.0, -0.5, 0.0)
    );
    var colors = array<vec4<f32>, 6>(
        vec4<f32>(1.0, 0.0, 0.0, 1.0),
        vec4<f32>(0.0, 1.0, 0.0, 1.0),
        vec4<f32>(0.0, 0.0, 1.0, 1.0),
        vec4<f32>(1.0, 1.0, 0.0, 1.0),
        vec4<f32>(1.0, 0.0, 1.0, 1.0),
        vec4<f32>(0.0, 1.0, 1.0, 1.0)
    );

    var output : VertexOutput;
    output.position = vec4<f32>(positions[vertexIndex], 1.0);
    output.color = colors[vertexIndex];
    return output;
}
