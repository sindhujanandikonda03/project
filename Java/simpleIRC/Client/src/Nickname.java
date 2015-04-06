
import java.awt.*;
import javax.swing.border.*;
import java.net.*;
import javax.swing.*;

import java.awt.event.*;

/**
 * class for generation the user config window
 * users specify there information and identity here
 */
public class Nickname extends JDialog {
	JPanel panelNick = new JPanel();
	JPanel panelSave = new JPanel();
	JButton save = new JButton();
	JButton cancel = new JButton();

	JLabel nick_l = new JLabel();
	
	String nick;
	String topic;
	JTextField Nickname;
	
	public Nickname(JFrame frame,String str) {
		super(frame, true);
		this.nick = str;
		try {
			jbInit();
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		//make the dialogue box centered
		Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
		this.setLocation( (int) (screenSize.width - 400) / 2 + 50,
						(int) (screenSize.height - 600) / 2 + 150);
		this.setResizable(false);
	}

	private void jbInit() throws Exception {
		this.setSize(new Dimension(150, 100));
		
		Nickname = new JTextField(10);
		Nickname.setText(nick);
		
		save.setText("save");
		cancel.setText("cancel");

		panelNick.add(nick_l);
		panelNick.add(Nickname);

		panelSave.add(new Label("              "));
		panelSave.add(save);
		panelSave.add(cancel);
		panelSave.add(new Label("              "));

		Container contentPane = getContentPane();
		contentPane.setLayout(new BorderLayout());
		contentPane.add(panelNick, BorderLayout.NORTH);
		contentPane.add(panelSave, BorderLayout.SOUTH);

		//event handling for save button
		save.addActionListener(
			new ActionListener() {
				public void actionPerformed (ActionEvent a) {
					if(Nickname.getText().equals("")){
						Nickname.setText(" username cant be empty!");
						Nickname.setText(nick);
						return;
					}
					else if(Nickname.getText().length() > 15){
						Nickname.setText(" username length cant exceed 15!");
						Nickname.setText(nick);
						return;
					}
					
					nick = Nickname.getText();
					dispose();
				}
			}
		);

		//event handler for dialogue box closing
		this.addWindowListener(
			new WindowAdapter(){
				public void windowClosing(WindowEvent e){
				}
			}
		);

		//event handler for cancel
		cancel.addActionListener(
			new ActionListener(){
				public void actionPerformed(ActionEvent e){
					dispose();
				}
			}
		);
	}
}