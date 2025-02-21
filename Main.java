import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.InputStreamReader;

public class Main {
    static {
        System.loadLibrary("ACOAlgorithm");
    }

    private native String runACOAlgorithm(String inputData);

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> createAndShowGUI());
    }

    private static void createAndShowGUI() {
        JFrame frame = new JFrame("Monochromatic Triangle Solver (ACO)");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(400, 300);

        JPanel panel = new JPanel();
        panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));

        JButton browseButton = new JButton("Browse");
        JButton runButton = new JButton("Run Algorithm");
        JTextArea resultArea = new JTextArea(10, 30);
        resultArea.setEditable(false);

        panel.add(browseButton);
        panel.add(Box.createRigidArea(new Dimension(0, 10)));
        panel.add(runButton);
        panel.add(Box.createRigidArea(new Dimension(0, 10)));
        panel.add(new JScrollPane(resultArea));

        browseButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                JFileChooser fileChooser = new JFileChooser();
                int result = fileChooser.showOpenDialog(frame);
                if (result == JFileChooser.APPROVE_OPTION) {
                    String filePath = fileChooser.getSelectedFile().getPath();
                    resultArea.setText("Selected file: " + filePath);
                }
            }
        });

        runButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String inputData = resultArea.getText();
                String result = runACOAlgorithm(inputData);
                resultArea.setText(result);

                // Call Python visualization script
                try {
                    ProcessBuilder pb = new ProcessBuilder("python", "visualize.py", result);
                    Process p = pb.start();
                    BufferedReader reader = new BufferedReader(new InputStreamReader(p.getInputStream()));
                    String line;
                    while ((line = reader.readLine()) != null) {
                        System.out.println(line);
                    }
                } catch (Exception ex) {
                    ex.printStackTrace();
                }
            }
        });

        frame.getContentPane().add(panel);
        frame.setVisible(true);
    }
}
