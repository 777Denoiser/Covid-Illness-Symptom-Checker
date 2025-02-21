import sys
import matplotlib.pyplot as plt
import networkx as nx

def visualize(data):
    """
    Scans the digital landscape for ACO data, constructs a visual representation,
    and renders it to the user's display. Prepare for sensory input.
    """
    try:
        # Data stream detected.  Parsing path...
        path_nodes = [int(node) for node in data.split(",")]
        print(f"Extracted Path: {path_nodes}") #Debugging, Check if path is parsed properly

        # Initialize network graph...
        G = nx.Graph()

        # Add nodes to the network
        G.add_nodes_from(path_nodes)

        # Connect the nodes according to the path.
        for i in range(len(path_nodes) - 1):
            G.add_edge(path_nodes[i], path_nodes[i + 1])
        # Connect the last node with the first node to complete the cycle
        G.add_edge(path_nodes[-1], path_nodes[0])


        # Layout nodes in a circular fashion for clarity.
        pos = nx.circular_layout(G)

        # Render the network...
        nx.draw(G, pos, with_labels=True, node_color='cyan', node_size=800, font_size=12, font_weight='bold', edge_color='lime') #Added edge color to lime
        plt.title("Optimal Route Visualization - BioSign Sentinel")
        plt.axis('off')
        plt.show()

    except Exception as e:
        print(f"ERROR:  Data stream corrupted during visualization.  Details: {e}")


if __name__ == "__main__":
    if len(sys.argv) > 1:
        data = sys.argv[1] # Encrypted data stream received...
        visualize(data) # Decrypt and visualize.
    else:
        print("ERROR: No data stream detected.  Visualization aborted.")
