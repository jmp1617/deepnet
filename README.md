# Deepnet
Multipurpose modular deep learning neural network with OpenGl visualization.<br />

Watch the neural net learn. Synaptic weight visualization with OpenGL allows the user to watch<br />
the neural net train.

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

Primative coordinate system: ( x: synapse0, y: synapse1, z: synapse2 )

# Images - Training 100000 iterations
 
## Default after 1.9%
![Default](/images/default.png)<br />
Each colored trail is one layer0 -> layer1 synaptic weight.
## SPEC enabled after 3.3%
![Spec](/images/spec.png)<br />
Each dot is one layer0 -> layer1 synaptic weight.
## LINE enabled after 2.1%
![Line](/images/line.png)<br />
Connect multiple synaptic weights with lines.
## TRI enabled after 1.7%
![Tri](/images/tri.png)<br />
Connect multiple synaptic weights with triangles.
