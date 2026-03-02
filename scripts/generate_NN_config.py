import json
import os

# List of available neuron types
NEURON_TYPES = ["PERCEPTRON", "RELU", "SIGMOID", "TANH"]

def get_int(prompt):
    while True:
        try:
            value = int(input(prompt))
            if value <= 0:
                print("Please enter a positive integer.")
                continue
            return value
        except ValueError:
            print("Invalid input. Please enter an integer.")

def get_choice(prompt, options):
    print(prompt)
    for i, option in enumerate(options, 1):
        print(f"{i}. {option}")
    while True:
        choice = input("Your choice: ")
        try:
            index = int(choice)
            if 1 <= index <= len(options):
                return options[index-1]
            else:
                print("Invalid number. Try again.")
        except ValueError:
            print("Invalid input. Try again.")

def main():
    layers = []

    print("=== Neural Network Configuration ===")
    
    # Number of hidden layers
    num_hidden = get_int("Number of hidden layers: ")

    for i in range(num_hidden):
        print(f"\n--- Hidden Layer {i+1} ---")
        size = get_int("Number of neurons: ")
        neuron_type = get_choice("Neuron type:", NEURON_TYPES)
        layers.append({"type": "hidden", "size": size, "neuron_type": neuron_type})

    # Output layer
    #print("\n--- Output Layer ---")
    #output_activation = get_choice("Activation function:", NEURON_TYPES)
    layers.append({"type": "output", "size": 5, "neuron_type": "SOFTMAX"})

    network = {"layers": layers}

    # Ask user for output file path
    while True:
        filename = input("\nPath to save the JSON file: ")
        script_path = os.path.abspath(__file__)
        current_dir = os.path.dirname(script_path)
        parent_dir = os.path.dirname(current_dir)
        dir_path = os.path.join(parent_dir, "NN_Config")
        filepath = os.path.join(dir_path, filename)

        
        if not os.path.exists(dir_path):
            print("Directory does not exist. Please enter a valid path.")
        else:
            break

    # Save JSON
    with open(filepath, "w") as f:
        json.dump(network, f, indent=2)

    print(f"\nFile '{filepath}' successfully generated!")
    print(json.dumps(network, indent=2))

if __name__ == "__main__":
    main()
