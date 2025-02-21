import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.File;

public class Main {
    static {
        System.loadLibrary("ACOAlgorithm"); // The ghost in the machine.
    }

    private native String runACOAlgorithm(String inputData);

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> createAndShowGUI());
    }

    private static void createAndShowGUI() {
        JFrame frame = new JFrame("Monochromatic Triangle Solver (ACO) - BioSign Sentinel"); // Give it a cool name
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(600, 400); // Enlarged the window

        JPanel panel = new JPanel();
        panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));
        panel.setBorder(BorderFactory.createEmptyBorder(20, 20, 20, 20)); // Add some padding

        // Set a cyberpunk-ish color scheme
        Color backgroundColor = new Color(0x1E1E1E); // Dark gray
        Color buttonColor = new Color(0x00FF7F); // Bright green
        Color textColor = new Color(0xDCDCDC); // Light gray

        panel.setBackground(backgroundColor);

        JButton browseButton = new JButton("Locate City Data");
        browseButton.setForeground(textColor);
        browseButton.setBackground(buttonColor);
        browseButton.setAlignmentX(Component.CENTER_ALIGNMENT); // Center the button

        JButton runButton = new JButton("Initiate Swarm");
        runButton.setForeground(textColor);
        runButton.setBackground(buttonColor);
        runButton.setAlignmentX(Component.CENTER_ALIGNMENT); // Center the button

        JTextArea resultArea = new JTextArea(15, 40); // Increased size
        resultArea.setEditable(false);
        resultArea.setBackground(backgroundColor);
        resultArea.setForeground(textColor);
        JScrollPane scrollPane = new JScrollPane(resultArea); // Enable scrolling
        scrollPane.setAlignmentX(Component.CENTER_ALIGNMENT);

        panel.add(browseButton);
        panel.add(Box.createRigidArea(new Dimension(0, 10)));
        panel.add(runButton);
        panel.add(Box.createRigidArea(new Dimension(0, 10)));
        panel.add(scrollPane);

        browseButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                JFileChooser fileChooser = new JFileChooser();
                int result = fileChooser.showOpenDialog(frame);
                if (result == JFileChooser.APPROVE_OPTION) {
                    File selectedFile = fileChooser.getSelectedFile();
                    String filePath = selectedFile.getPath();
                    resultArea.setText("City data located: " + filePath + "\nReady to unleash the swarm...");
                }
            }
        });

        runButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String inputData = resultArea.getText();
                // Extract the file path from the text area. Assume it's on the first line after "City data located: "
                String filePath = null;
                String[] lines = inputData.split("\\n"); // Split by newline
                if (lines.length > 0 && lines[0].startsWith("City data located: ")) {
                    filePath = lines[0].substring("City data located: ".length());
                    filePath = filePath.trim(); // Remove leading/trailing whitespace
                }

                if (filePath == null || filePath.isEmpty()) {
                    resultArea.setText("ERROR: No city data file selected.  Browse to a valid city data source.");
                    return;
                }

                String result = runACOAlgorithm(filePath); // Pass the actual file path to C++

                resultArea.setText("ACO Algorithm running...\n" + result); // Display raw ACO data

                // Call Python visualization script... if it's still operational.
                try {
                    ProcessBuilder pb = new ProcessBuilder("python", "visualize.py", result);
                    Process p = pb.start();

                    BufferedReader reader = new BufferedReader(new InputStreamReader(p.getInputStream()));
                    String line;
                    while ((line = reader.readLine()) != null) {
                        System.out.println(line); // Debugging, check python output
                    }
                    int exitCode = p.waitFor();
                    if (exitCode != 0) {
                        System.err.println("Python script exited with code : " + exitCode);
                    }

                } catch (Exception ex) {
                    ex.printStackTrace();
                    resultArea.append("\nERROR:  Python visualization error.  Check console for traceback.");
                }
            }
        });

        frame.getContentPane().add(panel);
        frame.setVisible(true);
    }
}
