import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.File;

public class Main {
    static {
        System.loadLibrary("ACOAlgorithm");
    }

    private native String runACOAlgorithm(String inputData);

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> createAndShowGUI());
    }

    private static void createAndShowGUI() {
        JFrame frame = new JFrame("Monochromatic Triangle Solver (ACO) - BioSign Sentinel");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(600, 400);

        JPanel panel = new JPanel();
        panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));
        panel.setBorder(BorderFactory.createEmptyBorder(20, 20, 20, 20));

        Color backgroundColor = new Color(0x1E1E1E);
        Color buttonColor = new Color(0x00FF7F);
        Color textColor = new Color(0xDCDCDC);

        panel.setBackground(backgroundColor);

        JButton browseButton = new JButton("Locate City Data");
        browseButton.setForeground(textColor);
        browseButton.setBackground(buttonColor);
        browseButton.setAlignmentX(Component.CENTER_ALIGNMENT);

        JButton runButton = new JButton("Initiate Swarm");
        runButton.setForeground(textColor);
        runButton.setBackground(buttonColor);
        runButton.setAlignmentX(Component.CENTER_ALIGNMENT);

        JTextArea resultArea = new JTextArea(15, 40);
        resultArea.setEditable(false);
        resultArea.setBackground(backgroundColor);
        resultArea.setForeground(textColor);
        JScrollPane scrollPane = new JScrollPane(resultArea);
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
                String filePath = null;
                String[] lines = inputData.split("\\n");
                if (lines.length > 0 && lines[0].startsWith("City data located: ")) {
                    filePath = lines[0].substring("City data located: ".length());
                    filePath = filePath.trim();
                }

                if (filePath == null || filePath.isEmpty()) {
                    resultArea.setText("ERROR: No city data file selected. Browse to a valid city data source.");
                    return;
                }

                String result = runACOAlgorithm(filePath);

                if (result.startsWith("ERROR:")) {
                    resultArea.setText(result);  // Display error from C++ code
                    return;
                }

                resultArea.setText("ACO Algorithm running...\n" + result);

                try {
                    ProcessBuilder pb = new ProcessBuilder("python", "visualize.py", result);
                    Process p = pb.start();

                    BufferedReader reader = new BufferedReader(new InputStreamReader(p.getInputStream()));
                    String line;
                    while ((line = reader.readLine()) != null) {
                        System.out.println(line);
                    }
                    int exitCode = p.waitFor();
                    if (exitCode != 0) {
                        System.err.println("Python script exited with code : " + exitCode);
                    }

                } catch (Exception ex) {
                    ex.printStackTrace();
                    resultArea.append("\nERROR: Python visualization error. Check console for traceback.");
                }
            }
        });

        frame.getContentPane().add(panel);
        frame.setVisible(true);
    }
}
