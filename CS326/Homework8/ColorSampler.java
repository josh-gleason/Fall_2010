// Joshua Gleason
// CS326
// Homework 8
// Dec 7, 2010

// IMPORTANT! requires the file colors.txt

import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
import javax.swing.event.*;
import java.io.*;

public class ColorSampler extends JFrame
{
	protected PaintBox colorWindow;
	protected JLabel labelRed;
	protected JLabel labelGreen;
	protected JLabel labelBlue;
	protected JTextField tfRed;
	protected JTextField tfGreen;
	protected JTextField tfBlue;
	protected JButton buttonRedMinus;
	protected JButton buttonRedPlus;
	protected JButton buttonGreenMinus;
	protected JButton buttonGreenPlus;
	protected JButton buttonBlueMinus;
	protected JButton buttonBluePlus;
	protected JButton buttonSave;
	protected JButton buttonReset;
	protected JList listColors;
	protected MyColors colors[];

	protected String winTitle;
	protected int red, green, blue;
	protected int index = 0;

	public static void main(String argv [])
	{
		try
		{
			new ColorSampler("Color Sampler");
		}
		catch (IOException e)
		{
			System.out.println(e+"\n");
			System.out.println("Does colors.txt exist?");
		}
	}

	public ColorSampler(String title) throws IOException
	{
		super(title);
		winTitle = title;
		
		int streamlen = 0;

		FileInputStream stream = new FileInputStream("colors.txt");
		InputStreamReader reader = new InputStreamReader(stream);
		StreamTokenizer tokens = new StreamTokenizer(reader);
	
		// count number of colors in file
		String tempS;
		int tempI;
		while ( tokens.nextToken() != tokens.TT_EOF )
		{
			tempS = (String) tokens.sval;
			tokens.nextToken();
			tempI = (int) tokens.nval;
			tokens.nextToken();
			tempI = (int) tokens.nval;
			tokens.nextToken();
			tempI = (int) tokens.nval;

			streamlen++;
		}
		stream.close();

		if ( streamlen == 0 )
		{
			System.out.println("colors.txt is empty!");
			System.exit(0);
		}

		stream = new FileInputStream("colors.txt");
		reader = new InputStreamReader(stream);
		tokens = new StreamTokenizer(reader);
	
		colors = new MyColors[streamlen];

		String data[] = new String[streamlen];
		
		// read into array
		for ( int i = 0; i < streamlen; i++ )
		{
			colors[i] = new MyColors();

			tokens.nextToken();
			colors[i].name = (String) tokens.sval;
			tokens.nextToken();
			colors[i].red = (int) tokens.nval;
			tokens.nextToken();
			colors[i].green = (int) tokens.nval;
			tokens.nextToken();
			colors[i].blue = (int) tokens.nval;
	
			// for use in list
			data[i] = colors[i].name;
		}

		stream.close();

		setBounds(100, 100, 370, 370);
		addWindowListener(new WindowDestroyer());
		colorWindow = new PaintBox(this);
		
		colorWindow.red = colors[index].red;
		red = colors[index].red;
		colorWindow.green = colors[index].green;
		green = colors[index].green;
		colorWindow.blue = colors[index].blue;
		blue = colors[index].blue;
		
		labelRed = new JLabel("Red:");
		labelGreen = new JLabel("Green:");
		labelBlue = new JLabel("Blue:");
		tfRed = new JTextField(Integer.toString(red));
		tfGreen = new JTextField(Integer.toString(green));
		tfBlue = new JTextField(Integer.toString(blue));
		buttonRedMinus = new JButton("-");
		buttonRedPlus = new JButton("+");
		buttonGreenMinus = new JButton("-");
		buttonGreenPlus = new JButton("+");
		buttonBlueMinus = new JButton("-");
		buttonBluePlus = new JButton("+");
		buttonSave = new JButton("Save");
		buttonReset = new JButton("Reset");
		listColors = new JList(data);

		getContentPane().setLayout(null);
		getContentPane().add(colorWindow);
		getContentPane().add(labelRed);
		getContentPane().add(tfRed);
		getContentPane().add(labelGreen);
		getContentPane().add(tfGreen);
		getContentPane().add(labelBlue);
		getContentPane().add(tfBlue);
		getContentPane().add(buttonRedMinus);
		getContentPane().add(buttonRedPlus);
		getContentPane().add(buttonGreenMinus);
		getContentPane().add(buttonGreenPlus);
		getContentPane().add(buttonBlueMinus);
		getContentPane().add(buttonBluePlus);
		getContentPane().add(buttonSave);
		getContentPane().add(buttonReset);
		getContentPane().add(listColors);

		listColors.setSelectedIndex(index);
		listColors.addListSelectionListener(new ListHandler());

		colorWindow.setBounds(10, 10, 220, 150);

		labelRed.setBounds(10, 170, 60, 30);
		tfRed.setBounds(70, 170, 50, 30);
		tfRed.setEditable(false);
		buttonRedMinus.setBounds(130, 170, 45, 30);
		buttonRedPlus.setBounds(185, 170, 45, 30);
		buttonRedMinus.addActionListener(new ActionHandler());
		buttonRedPlus.addActionListener(new ActionHandler());

		labelGreen.setBounds(10, 210, 60, 30);
		tfGreen.setBounds(70, 210, 50, 30);
		tfGreen.setEditable(false);
		buttonGreenMinus.setBounds(130, 210, 45, 30);
		buttonGreenPlus.setBounds(185, 210, 45, 30);
		buttonGreenMinus.addActionListener(new ActionHandler());
		buttonGreenPlus.addActionListener(new ActionHandler());
		
		labelBlue.setBounds(10, 250, 60, 30);
		tfBlue.setBounds(70, 250, 50, 30);
		tfBlue.setEditable(false);
		buttonBlueMinus.setBounds(130, 250, 45, 30);
		buttonBluePlus.setBounds(185, 250, 45, 30);
		buttonBlueMinus.addActionListener(new ActionHandler());
		buttonBluePlus.addActionListener(new ActionHandler());

		buttonSave.setBounds(30, 300, 80, 30);
		buttonReset.setBounds(120, 300, 80, 30);
		buttonSave.addActionListener(new ActionHandler());
		buttonReset.addActionListener(new ActionHandler());

		listColors.setBounds(245, 10, 110, 330);

		setVisible(true);
	}

	// Define list listener
	private class ListHandler implements ListSelectionListener
	{
		public void valueChanged(ListSelectionEvent e)
		{
			if ( e.getSource() == listColors )	// which it should always :P
			{
				if ( !e.getValueIsAdjusting() )
				{
					index = listColors.getSelectedIndex();
					setTitle(winTitle);
					red = colors[index].red;
					green = colors[index].green;
					blue = colors[index].blue;
					colorWindow.red = red;
					colorWindow.green = green;
					colorWindow.blue = blue;
					tfRed.setText(Integer.toString(red));
					tfGreen.setText(Integer.toString(green));
					tfBlue.setText(Integer.toString(blue));
					repaint();
				}
			}
		}
	}

	// Define action listener
	private class ActionHandler implements ActionListener
	{
		public void actionPerformed(ActionEvent e)
		{
			if ( e.getSource() == buttonRedMinus )
			{
				if (red >= 5)
				{
					red -= 5;
					colorWindow.red = red;
					tfRed.setText(Integer.toString(red));
					setTitle(winTitle+"*");
					repaint();
				}
			}
			else if ( e.getSource() == buttonRedPlus )
			{
				if ( red <= 250 )
				{
					red += 5;
					colorWindow.red = red;
					tfRed.setText(Integer.toString(red));
					setTitle(winTitle+"*");
					repaint();
				}
			}
			else if ( e.getSource() == buttonGreenMinus )
			{
				if (green >= 5)
				{
					green -= 5;
					colorWindow.green = green;
					tfGreen.setText(Integer.toString(green));
					setTitle(winTitle+"*");
					repaint();
				}
			}
			else if ( e.getSource() == buttonGreenPlus )
			{
				if ( green <= 250 )
				{
					green += 5;
					colorWindow.green = green;
					tfGreen.setText(Integer.toString(green));
					setTitle(winTitle+"*");
					repaint();
				}
			}
			else if ( e.getSource() == buttonBlueMinus )
			{
				if (blue >= 5)
				{
					blue -= 5;
					colorWindow.blue = blue;
					tfBlue.setText(Integer.toString(blue));
					setTitle(winTitle+"*");
					repaint();
				}
			}
			else if ( e.getSource() == buttonBluePlus )
			{
				if ( blue <= 250 )
				{
					blue += 5;
					colorWindow.blue = blue;
					tfBlue.setText(Integer.toString(blue));
					setTitle(winTitle+"*");
					repaint();
				}
			}
			else if ( e.getSource() == buttonReset )
			{
				setTitle(winTitle);
				red = colors[index].red;
				green = colors[index].green;
				blue = colors[index].blue;
				colorWindow.red = red;
				colorWindow.green = green;
				colorWindow.blue = blue;
				tfRed.setText(Integer.toString(red));
				tfGreen.setText(Integer.toString(green));
				tfBlue.setText(Integer.toString(blue));
				repaint();
			}
			else if ( e.getSource() == buttonSave )
			{
				setTitle(winTitle);
				colors[index].red = red;
				colors[index].green = green;
				colors[index].blue = blue;
			}
		}
	}

	// Define window adapter                                       
	private class WindowDestroyer extends WindowAdapter 
	{      
		public void windowClosing(WindowEvent e)
		{    
			// save to file
			try
			{
				FileOutputStream stream = new FileOutputStream("colors.txt");
				PrintWriter writer = new PrintWriter(stream);

				for ( int i = 0; i < colors.length; i++ )
				{
					writer.println(
						colors[i].name + " " +
						colors[i].red + " " +
						colors[i].green + " " +
						colors[i].blue);
				}
				writer.flush();
				stream.close();
			}
			catch (IOException exception)
			{
				System.out.println(exception + "\n");
			}
			
			// exit
			System.exit(0);  
		}                                                             
	}                                                              
}

class PaintBox extends JComponent 
{
	ColorSampler app;
	int	x = -1;
	int	y = -1;
	int red, green, blue;

	public PaintBox(ColorSampler a)
	{
		app = a;
	}

	public void paint(Graphics g)
	{
		Dimension d = getSize();

		g.setColor(new Color(red/255f,green/255f,blue/255f));
		g.fillRect(1, 1, d.width-2, d.height-2);
	}
}

class MyColors
{
	public String name;
	public int red;
	public int green;
	public int blue;
}

