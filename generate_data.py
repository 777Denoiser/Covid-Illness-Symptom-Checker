import random

def generate_large_city_data(filename="large_cities.txt", num_cities=2000):
    """
    Generates a large city data file with random coordinates.

    Args:
        filename (str): The name of the file to create.
        num_cities (int): The number of cities to generate.
    """
    try:
        with open(filename, "w") as f:
            for i in range(1, num_cities + 1):
                x = random.randint(0, 1000)  # X coordinate range
                y = random.randint(0, 1000)  # Y coordinate range
                f.write(f"{i} {x} {y}\n")
        print(f"Successfully created large city data file: {filename} with {num_cities} cities.")
    except Exception as e:
        print(f"Error generating large city data file: {e}")

if __name__ == "__main__":
    generate_large_city_data() # You can customize the filename and number of cities
