import sys
import matplotlib.pyplot as plt
import networkx as nx


def visualize(data):
    # Parse the data string
    # Create a graph using networkx
    G = nx.Graph()
    # Add nodes and edges based on the parsed data

    # Draw the graph
    pos = nx.spring_layout(G)
    nx.draw(G, pos, with_labels=True, node_color='lightblue', node_size=500, font_size=10, font_weight='bold')
    edge_labels = nx.get_edge_attributes(G, 'weight')
    nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels)

    plt.title("Monochromatic Triangle Visualization")
    plt.axis('off')
    plt.show()


if __name__ == "__main__":
    if len(sys.argv) > 1:
        data = sys.argv[1]
        visualize(data)
    else:
        print("No data provided for visualization")
