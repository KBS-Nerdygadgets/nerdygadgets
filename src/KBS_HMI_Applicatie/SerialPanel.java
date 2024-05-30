package KBS_HMI_Applicatie;

import javax.swing.*;
import javax.swing.border.EmptyBorder;

import com.fazecast.jSerialComm.SerialPort;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;

public class SerialPanel extends JPanel {
    private JLabel rX, tX, rood, groen, rood1, groen1;
    private JButton connectionButton, settingsButton;
    private JComboBox<String> startButton;
    private JPanel serialStatus;
    private ArrayList<SerialPort> portsList = new ArrayList<>();
    public SerialPort selectedPort;
    private boolean connected;
    private Icon redIcon, greenIcon;


    public SerialPanel(Dimension screenDimension) {
        SerialComm();
        connected = false;

        //De kleuren die in dit paneel gebruikt worden
        Color background = new Color(42, 42, 42);
        Color buttonColor = new Color(73, 73, 73);
        Color buttonForegroundColor = new Color(200, 200, 200);
        Color foregroundColor = new Color(204, 204, 204);

        // De gebruikte dimensies
        Dimension buttonDimensions = new Dimension(125, 30);
        Dimension settingsButtonDimensions = new Dimension(30, 30);

        // Specificaties Seriele control paneel
        setBackground(background);
        setPreferredSize(screenDimension);
        setLayout(new BorderLayout());
        setBorder(new EmptyBorder(10, 10, 10, 10));

        // Maak linker paneel aan, Bevat select device, combobox en connect
        JPanel leftPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        leftPanel.setBackground(background);

        // Select device label
        JLabel select = new JLabel("Select device:    ");
        select.setForeground(foregroundColor);
        select.setFont(new Font(select.getFont().getName(), Font.PLAIN, 20));

        // Combobox aanmaken
        startButton = new JComboBox<>();
        startButton.setBackground(buttonColor);
        startButton.setForeground(buttonForegroundColor);
        startButton.setPreferredSize(buttonDimensions);
        startButton.setFont(new Font(startButton.getFont().getName(), Font.PLAIN, 20));

        // COM ports toevoegen aan combobox
        for (SerialPort port : portsList) {
            String tempPort = port.getDescriptivePortName();
            startButton.addItem(tempPort.substring(tempPort.length() - 5, tempPort.length() - 1));
        }

        // Connect button aanmaken
        connectionButton = new JButton("Connect");
        connectionButton.setBackground(buttonColor);
        connectionButton.setForeground(buttonForegroundColor);
        connectionButton.setPreferredSize(buttonDimensions);
        connectionButton.setFont(new Font(connectionButton.getFont().getName(), Font.PLAIN, 20));
        connectionButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                connectToSelectedPort();
            }
        });

        // Paneel aanmaken voor het weergeven van de verbinding met de port
        serialStatus = new JPanel(new GridLayout(2, 2));
        serialStatus.setBackground(background);
        serialStatus.setPreferredSize(new Dimension(80, 40));

        // Labels en gekleurde cirkels voor de seriele status aanmaken
        rX = new JLabel("RX");
        rX.setForeground(foregroundColor);
        rX.setHorizontalAlignment(SwingConstants.CENTER);
        tX = new JLabel("TX");
        tX.setForeground(foregroundColor);
        tX.setHorizontalAlignment(SwingConstants.CENTER);
        redIcon = new ImageIcon("src/KBS_HMI_Applicatie/Icons/redCircle.png");
        rood = new JLabel(redIcon);
        rood1 = new JLabel(redIcon);
        greenIcon = new ImageIcon("src/KBS_HMI_Applicatie/Icons/greenCircle.png");
        groen = new JLabel(greenIcon);
        groen1 = new JLabel(greenIcon);

        serialStatus.add(rX);
        serialStatus.add(tX);
        serialStatus.add(rood);
        serialStatus.add(rood1);

        // Rechter paneel voor de settings knop 
        JPanel rightPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        rightPanel.setBackground(background);

        // Settings knop aanmaken
        Icon settingsIcon = new ImageIcon("src/KBS_HMI_Applicatie/Icons/settingsIcon.png");
        settingsButton = new JButton(settingsIcon);
        settingsButton.setBackground(buttonColor);
        settingsButton.setPreferredSize(settingsButtonDimensions);

        // Alle panelen toevoegen
        leftPanel.add(select);
        leftPanel.add(startButton);
        leftPanel.add(connectionButton);
        leftPanel.add(serialStatus);

        rightPanel.add(settingsButton);

        add(leftPanel, BorderLayout.WEST);
        add(rightPanel, BorderLayout.EAST);
    }

    public void SerialComm() {
        // Lijst met ports
        SerialPort[] ports = SerialPort.getCommPorts();

        if (ports.length == 0) {
            System.out.println("No ports found!");
            return;
        }

        // Sla ports op
        for (SerialPort port : ports) {
            portsList.add(port);
        }
    }

    public void connectToSelectedPort() {
        int selectedIndex = startButton.getSelectedIndex();
        if (selectedIndex == -1) {
            JOptionPane.showMessageDialog(this, "Please select a port.", "Error", JOptionPane.ERROR_MESSAGE);
            return;
        }

        SerialPort selectedPort = portsList.get(selectedIndex);

        // Open poort
        if (selectedPort.openPort()) {
            System.out.println("Serial port successfully opened.");
            connected = true;
            // Verander iconen naar groen
            rood.setIcon(greenIcon);
            rood1.setIcon(greenIcon);
        } else {
            System.out.println("Error, could not open port.");
            JOptionPane.showMessageDialog(this, "Failed to open port " + selectedPort.getDescriptivePortName(), "Error", JOptionPane.ERROR_MESSAGE);
            connected = false;
            // Verander iconen naar rood
            rood.setIcon(redIcon);
            rood1.setIcon(redIcon);
            return;
        }

        // Set baud rate
        selectedPort.setBaudRate(9600);

        // Poort sluiten wanneer klaar
        selectedPort.closePort();
    }
    public void sendToSelectedPort(String dataToSend) {
        int selectedIndex = startButton.getSelectedIndex();
        if (selectedIndex == -1) {
            JOptionPane.showMessageDialog(this, "Please select a port.", "Error", JOptionPane.ERROR_MESSAGE);
            return;
        }

        SerialPort selectedPort = portsList.get(selectedIndex);

        // Open poort
        if (selectedPort.openPort()) {
            System.out.println("Serial port successfully opened.");
            connected = true;
            // Verander iconen naar groen
            rood.setIcon(greenIcon);
            rood1.setIcon(greenIcon);
        } else {
            System.out.println("Error, could not open port.");
            JOptionPane.showMessageDialog(this, "Failed to open port " + selectedPort.getDescriptivePortName(), "Error", JOptionPane.ERROR_MESSAGE);
            connected = false;
            // Verander iconen naar rood
            rood.setIcon(redIcon);
            rood1.setIcon(redIcon);
            return;
        }

        // Set baud rate
        selectedPort.setBaudRate(9600);
        selectedPort.writeBytes(dataToSend.getBytes(), dataToSend.getBytes().length);
        System.out.println("Serial port successfully sent.");
        // Poort sluiten wanneer klaar
        selectedPort.closePort();
    }
}