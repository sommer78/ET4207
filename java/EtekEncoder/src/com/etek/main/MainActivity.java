package com.etek.main;

import java.awt.EventQueue;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JEditorPane;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;

import com.etek.util.Encode;

public class MainActivity {

	private JFrame frame;

	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					MainActivity window = new MainActivity();
					window.frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the application.
	 */
	public MainActivity() {
		initialize();
	}

	/**
	 * Initialize the contents of the frame.
	 */
	private void initialize() {
		frame = new JFrame();
		frame.setBounds(100, 100, 1200, 700);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.getContentPane().setLayout(null);
		frame.setExtendedState(JFrame.MAXIMIZED_BOTH);

		JLabel lblConsumerIrUs = new JLabel("consumer ir (us)");
		lblConsumerIrUs.setBounds(31, 28, 129, 21);
		frame.getContentPane().add(lblConsumerIrUs);

		JEditorPane textAreaConsumerIrUs = new JEditorPane();
		textAreaConsumerIrUs.setBounds(31, 59, 515, 39);
		frame.getContentPane().add(textAreaConsumerIrUs);

		JLabel lblConsumerIrCarrier = new JLabel("consumer ir (载波)");
		lblConsumerIrCarrier.setBounds(31, 108, 141, 15);
		frame.getContentPane().add(lblConsumerIrCarrier);

		JEditorPane textAreaConsumerIrCarrier = new JEditorPane();
		textAreaConsumerIrCarrier.setBounds(31, 133, 515, 120);
		frame.getContentPane().add(textAreaConsumerIrCarrier);

		JLabel lblEtek = new JLabel("ETEK格式");
		lblEtek.setBounds(31, 263, 78, 15);
		frame.getContentPane().add(lblEtek);

		JEditorPane lblEtekShow = new JEditorPane();
		lblEtekShow.setBounds(31, 288, 557, 164);
		frame.getContentPane().add(lblEtekShow);

		JButton startChange = new JButton("开始转换");
		startChange.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String code = null;
				int freq;
				int[] data;
				if ((!textAreaConsumerIrUs.getText().isEmpty())
						&& (textAreaConsumerIrCarrier.getText().isEmpty())) {
					String[] tempValues = textAreaConsumerIrUs.getText().split(
							",");
					Integer[] values = new Integer[tempValues.length];
					for (int n = 0; n < tempValues.length; n++) {
						values[n] = Integer.valueOf(tempValues[n]);
					}
					freq = values[0];
					data = new int[values.length - 1];
					for (int n = 0; n < values.length - 1; n++) {
						data[n] = values[n + 1] * freq / 1000000;
					}
//					System.out.println("freq = " + freq);
//					for (int dat : data) {
//						System.out.println(dat);
//					}
					code = Encode.encodeToEtek(freq, data);

				} else if ((textAreaConsumerIrUs.getText().isEmpty())
						&& (!textAreaConsumerIrCarrier.getText().isEmpty())) {
					String[] tempValues = textAreaConsumerIrCarrier.getText().split(
							",");
					Integer[] values = new Integer[tempValues.length];
					for (int n = 0; n < tempValues.length; n++) {
						values[n] = Integer.valueOf(tempValues[n]);
					}
					freq = values[0];
					data = new int[values.length - 1];
					for (int n = 0; n < values.length - 1; n++) {
						data[n] = values[n + 1];
					}
//					System.out.println("freq = " + freq);
//					for (int dat : data) {
//						System.out.println(dat);
//					}
					code = Encode.encodeToEtek(freq, data);
				} else {
					JOptionPane.showMessageDialog(frame, "请保证只有一个栏中有数据！",
							"ERROR", JOptionPane.ERROR_MESSAGE);
				}

				lblEtekShow.setText(code);
			}
		});
		startChange.setBounds(298, 27, 93, 23);
		frame.getContentPane().add(startChange);
		
		JLabel lblJson = new JLabel("Json格式");
		lblJson.setBounds(31, 462, 78, 15);
		frame.getContentPane().add(lblJson);
		
		JEditorPane editorPane = new JEditorPane();
		editorPane.setBounds(31, 487, 557, 165);
		frame.getContentPane().add(editorPane);
	}
}
