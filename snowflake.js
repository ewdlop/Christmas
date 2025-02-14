async function init() {
    if (!navigator.gpu) {
        console.error("WebGPU is not supported!");
        return;
    }

    const adapter = await navigator.gpu.requestAdapter();
    const device = await adapter.requestDevice();

    const canvas = document.getElementById('canvas');
    const context = canvas.getContext('webgpu');

    const swapChainFormat = "bgra8unorm";
    context.configure({
        device: device,
        format: swapChainFormat
    });

    const vertexModule = device.createShaderModule({
        code: `
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
        `
    });

    const fragmentModule = device.createShaderModule({
        code: `
        @fragment
        fn main(@location(0) color : vec4<f32>) -> @location(0) vec4<f32> {
            return color;
        }
        `
    });

    const pipeline = device.createRenderPipeline({
        vertex: {
            module: vertexModule,
            entryPoint: "main"
        },
        fragment: {
            module: fragmentModule,
            entryPoint: "main",
            targets: [{
                format: swapChainFormat
            }]
        },
        primitive: {
            topology: "triangle-list"
        }
    });

    function frame() {
        const commandEncoder = device.createCommandEncoder();
        const textureView = context.getCurrentTexture().createView();
        const renderPassDescriptor = {
            colorAttachments: [{
                view: textureView,
                loadValue: {r: 0, g: 0, b: 0, a: 1},
                storeOp: "store"
            }]
        };

        const passEncoder = commandEncoder.beginRenderPass(renderPassDescriptor);
        passEncoder.setPipeline(pipeline);
        passEncoder.draw(6, 1, 0, 0);
        passEncoder.endPass();

        device.queue.submit([commandEncoder.finish()]);

        requestAnimationFrame(frame);
    }

    requestAnimationFrame(frame);
}

window.onload = init;
