package KBS_HMI_Applicatie;

import javax.swing.*;
import java.awt.*;

public class GUIManager extends JFrame {
    private SerialConnectionManager scmPanel;
    private ControlPanel controlPanel;
    private StatusPanel statusPanel;
    private OrderPanel orderPanel;
    public GUIManager(Dimension dimension){
        Color darkGray1 = new Color(51, 51, 51);

        setTitle("HMI");
        setSize(dimension);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        Dimension lrPanelDimension = new Dimension(dimension.width/2-7, dimension.height-50);

        scmPanel = new SerialConnectionManager(new Dimension(dimension.width, 65));
        statusPanel = new StatusPanel(new Dimension(lrPanelDimension.width, lrPanelDimension.height/3*2));
        orderPanel = new OrderPanel(new Dimension(lrPanelDimension.width, lrPanelDimension.height/3));

        JPanel leftPanel = new JPanel();
        leftPanel.setBackground(Color.lightGray);
        leftPanel.setPreferredSize(lrPanelDimension);

        JPanel rightPanel = new JPanel();
        rightPanel.setLayout(new BorderLayout());
        rightPanel.setBackground(Color.green);
        rightPanel.setPreferredSize(lrPanelDimension);

//        JPanel topPanel = new JPanel();
//        topPanel.setBackground(Color.RED);
//        topPanel.setPreferredSize(new Dimension(lrPanelDimension.width/2, lrPanelDimension.height/3*2));
//
//        JPanel bottomPanel = new JPanel();
//        bottomPanel.setBackground(Color.blue);
//        bottomPanel.setPreferredSize(new Dimension(lrPanelDimension.width/2, lrPanelDimension.height/3));
//
//        rightPanel.add(topPanel, BorderLayout.NORTH);
//        rightPanel.add(bottomPanel, BorderLayout.CENTER);

        add(scmPanel, BorderLayout.NORTH);
        add(leftPanel, BorderLayout.WEST);
        add(rightPanel, BorderLayout.EAST);

        rightPanel.add(statusPanel, BorderLayout.NORTH);
        rightPanel.add(orderPanel, BorderLayout.CENTER);

        setVisible(true);
    }
    public SerialConnectionManager getSerialConnectionManager(){
        return scmPanel;
    }
    public ControlPanel getControlPanel(){
        return controlPanel;
    }
    public StatusPanel getStatusPanel(){
        return statusPanel;
    }
}
