import json
import os

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

def get_float(prompt):
    while True:
        try:
            value = float(input(prompt))
            if value <= 0:
                print("Please enter a positive number.")
                continue
            return value
        except ValueError:
            print("Invalid input. Please enter a number.")

def main():
    print("=== Training Configuration Generator ===")
    
    epochs = get_int("Number of epochs: ")
    mini_batch_size = get_int("Mini-batch size: ")
    eta = get_float("Learning rate (eta): ")

    config = {
        "epochs": epochs,
        "mini_batch_size": mini_batch_size,
        "eta": eta
    }

    # Ask user for output file path
    while True:
        modelname = input("\nEnter the name of the model you're building this training config for: ")
        filename = input("Enter the name of the training config file: ")
        script_path = os.path.abspath(__file__)
        current_dir = os.path.dirname(script_path)
        parent_dir = os.path.dirname(current_dir)
        dir_path = os.path.join(parent_dir, "NN_Training_Config")
        final_dir = os.path.join(dir_path, modelname)
        last_dir = os.path.join(final_dir, filename)

        if not os.path.exists(final_dir):
            os.makedirs(final_dir)
        break

    # Save JSON
    with open(last_dir, "w") as f:
        json.dump(config, f, indent=4)

    print(f"\nFile '{last_dir}' successfully generated!")
    print(json.dumps(config, indent=4))

if __name__ == "__main__":
    main()
