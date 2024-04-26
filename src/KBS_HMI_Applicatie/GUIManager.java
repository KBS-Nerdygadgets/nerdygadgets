package KBS_HMI_Applicatie;

import javax.swing.*;
import java.awt.*;

public class GUIManager extends JFrame {
    private SerialPanel scmPanel;
    private ControlPanel controlPanel;
    private StatusPanel statusPanel;
    private OrderPanel orderPanel;
    public GUIManager(Dimension dimension){
        //De kleuren die in dit paneel gebruikt worden. Deze worden met de integratie van themas aangepast
        Color darkGray = new Color(51, 51, 51);
        //De dimensions die gebruikt worden
        Dimension lrPanelDimension = new Dimension(dimension.width/2-7, dimension.height-50);

        //Past het Frame aan
        setTitle("HMI");
        setSize(dimension);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());
        setBackground(darkGray);
        ImageIcon hasbulla = new ImageIcon("src/KBS_HMI_Applicatie/Icons/hasbie.jpeg");
        setIconImage(hasbulla.getImage());

        //De verschillende panelen worden aangemaakt
        scmPanel = new SerialPanel(new Dimension(dimension.width, 65));
        statusPanel = new StatusPanel(new Dimension(lrPanelDimension.width, lrPanelDimension.height/3*2));
        orderPanel = new OrderPanel(new Dimension(lrPanelDimension.width, lrPanelDimension.height/3));
        controlPanel = new ControlPanel(lrPanelDimension);

        //Het linker paneel die de status panel en order panel zal bevatten wordt aangemaakt
        JPanel leftPanel = new JPanel();
        leftPanel.setLayout(new BorderLayout());
        leftPanel.setBackground(Color.lightGray);
        leftPanel.setPreferredSize(lrPanelDimension);

        //Het rechter paneel dat het schap en zijn legende zal bevatten wordt aangemaakt
        JPanel rightPanel = new JPanel(new BorderLayout());
        rightPanel.setPreferredSize(lrPanelDimension);

        //Alle panelen worden toegevoegd aan de GUI
        add(scmPanel, BorderLayout.NORTH);
        add(leftPanel, BorderLayout.WEST);
        add(rightPanel, BorderLayout.EAST);

        leftPanel.add(controlPanel, BorderLayout.CENTER);

        rightPanel.add(statusPanel, BorderLayout.NORTH);
        rightPanel.add(orderPanel, BorderLayout.CENTER);

        setVisible(true);
    }
    public SerialPanel getSerialConnectionManager(){
        return scmPanel;
    }
    public ControlPanel getControlPanel(){
        return controlPanel;
    }
    public StatusPanel getStatusPanel(){
        return statusPanel;
    }
}
