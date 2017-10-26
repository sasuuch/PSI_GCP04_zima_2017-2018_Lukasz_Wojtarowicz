public class Layer {
    Neuron neurons[];
    int numberOfNeurons;

    public Layer(int numberOfNeurons){
        this.numberOfNeurons = numberOfNeurons;
        this.neurons= new Neuron[numberOfNeurons];

        for(int i=0; i<numberOfNeurons ; i++){
            this.neurons[i] = new Neuron();
        }
    }
}