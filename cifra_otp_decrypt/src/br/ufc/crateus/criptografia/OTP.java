package br.ufc.crateus.criptografia;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class OTP {
	static ArrayList<String> mens1 = new ArrayList<>();
	static ArrayList<String> mens2 = new ArrayList<>();
	static ArrayList<String> mens3 = new ArrayList<>();

	static char[] plaintext1 = new char[156];
	static char[] plaintext2 = new char[156];
	static char[] plaintext3 = new char[156];

	static int cont = 0;
	static Scanner scan = new Scanner(System.in);

	@SuppressWarnings("resource")
	public static void main(String[] args) throws FileNotFoundException, IOException {
		Scanner s = new Scanner(System.in);

		RandomAccessFile read1 = new RandomAccessFile(
				"/home/marcus/Documentos/CC - semestre 2017.2/criptografia/Criptografia/ct1.tex", "rw");
		RandomAccessFile read2 = new RandomAccessFile(
				"/home/marcus/Documentos/CC - semestre 2017.2/criptografia/Criptografia/ct2.tex", "rw");
		RandomAccessFile read3 = new RandomAccessFile(
				"/home/marcus/Documentos/CC - semestre 2017.2/criptografia/Criptografia/ct3.tex", "rw");

		byte[] m1 = new byte[(int) read1.length()];
		byte[] m2 = new byte[(int) read2.length()];
		byte[] m3 = new byte[(int) read2.length()];
		byte[] m4 = new byte[m1.length];
		byte[] m5 = new byte[m1.length];
		byte[] m6 = new byte[m1.length];

		int x = 0;
		read1.seek(0);
		read2.seek(0);
		read3.seek(0);
		while (x < (int) read1.length()) {
			read1.read(m1);
			read2.read(m2);
			read3.read(m3);
			x++;
		}

		m4 = xor(m1, m2);
		m5 = xor(m1, m3);
		m6 = xor(m2, m3);

		for (int i = 0; i < 156; i++) {
			plaintext1[i] = '*';
			plaintext2[i] = '*';
			plaintext3[i] = '*';

		}

		String position = "s";
		String aux;
		int count = 0, inicio, fim;
		while (position.equals("s")) {
			System.out.println("Buscar texto:");
			String c = s.nextLine();
			System.out.println("arquivos 1 e 2:");
			combina(m4, c);
			System.out.println("\narquivos 1 e 3:");
			combina(m5, c);
			System.out.println("\narquivos 2 e 3:");
			combina(m6, c);
			System.out.println("\nPossibilidades mensagem 1:");

			count = Integer.valueOf(s.nextLine());

			while (count-- > 0) {
				aux = s.nextLine();
				inicio = Integer.valueOf(s.nextLine());
				fim = Integer.valueOf(s.nextLine());
				save(inicio, fim, aux, 1);
			}

			// textoClaro(count, mens1);
			System.out.println("\nPossibilidades mensagem 2:");
			count = Integer.valueOf(s.nextLine());
			while (count-- > 0) {
				aux = s.nextLine();
				inicio = Integer.valueOf(s.nextLine());
				fim = Integer.valueOf(s.nextLine());
				save(inicio, fim, aux, 2);
			}
			// textoClaro(count, mens2);
			System.out.println("\nPossibilidades mensagem 3:");
			count = Integer.valueOf(s.nextLine());

			while (count-- > 0) {
				aux = s.nextLine();
				inicio = Integer.valueOf(s.nextLine());
				fim = Integer.valueOf(s.nextLine());
				save(inicio, fim, aux, 3);
			}
			// textoClaro(count, mens3);
			System.out.println("Texto Claro 1: ");
			print(plaintext1);
			System.out.println("Texto Claro 2: ");
			print(plaintext2);
			System.out.println("Texto Claro 3: ");
			print(plaintext3);
			System.gc();
			System.out.println("Processeguir?[s/n]");
			position = s.nextLine();

		}
	}

	public static boolean getOnlyASCII(String raw) {
		Pattern asciiPattern = Pattern.compile("^[a-zA-Z\\,\\s]+$");
		Matcher matcher = asciiPattern.matcher(raw);
		String asciiString = null;
		if (matcher.find()) {
			asciiString = matcher.group();
		}

		if (asciiString == null)
			return false;

		return asciiString.equals(raw);
	}

	public static void combina(byte[] a, String c) {
		String st = "";
		int count = 0, control = 0;
		cont = 0;
		for (int j = 0; j < a.length && count < a.length; j++) {
			count = j;
			for (int i = 0; count < a.length && i < c.length(); i++) {
				st += String.valueOf((char) (byte) (a[count++] ^ c.charAt(i)));
				control++;
				if (control == c.length()) {
					if (getOnlyASCII(st)) {
						System.out.println(st + " " + cont + " " + (c.length() - 1 + cont++));
					} else {
						cont++;
					}
					st = "";
					control = 0;
				}
			}
		}

	}

	public static void save(int inicio, int fim, String aux, int text) {

		if (text == 1) {
			for (int i = inicio; i <= fim; i++) {
				plaintext1[i] = aux.charAt(i - inicio);
			}

		} else if (text == 2) {
			for (int i = inicio; i <= fim; i++)
				plaintext2[i] = aux.charAt(i - inicio);

		} else if (text == 3) {
			for (int i = inicio; i <= fim; i++)
				plaintext3[i] = aux.charAt(i - inicio);

		}

	}

	public static void textoClaro(int num, ArrayList<String> mensagem) {
		for (int i = 0; i < num; i++) {
			mensagem.add(scan.nextLine());
		}

	}

	public static void print(char[] v) {
		for (int i = 0; i < 156; i++) {
			System.out.print(v[i]);
		}
		System.out.println();
	}

	public static byte[] xor(byte[] m, byte[] n) {
		byte[] resutado = new byte[m.length];
		for (int i = 0; i < m.length; i++) {
			try {
				resutado[i] = (byte) (m[i] ^ n[i]);
			} catch (Exception e) {

			}
		}

		return resutado;
	}

}