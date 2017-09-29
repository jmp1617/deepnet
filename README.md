# Deepnet
Multipurpose modular deep learning neural network with OpenGl visualization.<br />

## Dependencies
GLFW 3, OPENGL, GLEW.<br />

## Usage
Usage: deepnet /[mode] /[# data] /[size] /[neurons.brain] /[visualize]<br />

mode: /[t/a] either a 't' to specify training mode or 'a' to analyze. <br />
/# data:  number of data for training or analysis.<br />
size: the length of the largest data (including solution flag and length header if training).<br />
neurons.brain: file to dump neurons to after training or load neurons for analysis.<br />
visualize: [0 to # data] opengl visualization of training (slower). 0 to disable.<br />

## Visualization option
Make targets: spec, line, and tri are available.<br />
    
spec - Screen clear after each render. Just shows current synapse values.<br />
line - OpenGL lines.<br />
tri - OpenGL triangles.<br />
