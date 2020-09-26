
import java.awt.Component;
import java.awt.event.ActionEvent;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.stream.Collectors;
import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.swing.table.DefaultTableModel;
import javax.swing.table.TableCellRenderer;

/**
 *
 * @author paul
 */
public final class Window extends javax.swing.JFrame {

    private ArrayList<Cell> cells, optimizedPath;
    private ArrayList<Double> consumerPowers, producerPowers;
    private double[] consValues, prodValues;
    private final double unusedValue = 1e9;
    private double totalProd = 0, totalCon = 0;
    private final int MINUS = 1;
    private final int PLUS = 2;
    private final int HORIZONTAL = 3;
    private final int VERTICAL = 4;
    private Object[][] tableData;

    public Window() {
        initComponents();
        initState();
    }

    public void initState() {
        this.referenceMenuItem.addActionListener((ActionEvent e) -> {
            JOptionPane.showMessageDialog(null, "<html>"
                    + "    <h2>Справка</h2>"
                    + "    <p>Программа для решения невырожденной транспортной задачи открытого или закрытого типа</p>"
                    + "    <h3>Входные данные</h3>"
                    + "    <p>Программа поддерживает числа с плавающей запятой для значений тарифов на перевозку грузов.</p>"
                    + "    <p>Чтобы выбрать файл с входными данными нажмите на \"File\" -> \"Choose file\"<br>и в открывшемся диалоге выберите файл и нажмите \"Ok\"</p>"
                    + "    <h3>Постановка задачи</h3>"
                    + "    <p>После открытия входного файла в таблице появятся данные задачи - в заголовке таблицы<br>производители P1 {мощность производства}, P2 {мощность производства} ...<br>"
                    + "        В первой колонке таблицы обозначены потребители в формате C1 {мощность потребления}, C2 {мощность потребления} ...<br>"
                    + "        Справа от таблицы появится надпись \"task type: \" {значение}.<br>Значение \"default\" обзначает  закрытая транспортная задача;<br>"
                    + "        \"Over consuming\" означает что задача открытого типа и мощность потребления превышает мощность производства;<br>"
                    + "        \"Over production\" означает что задача открытого типа и мощность производства превышает мощность потребления;<br>"
                    + "    </p>"
                    + "    <h3>Получение решения</h3>"
                    + "    <p>Для решения задачи нажмите на \"Solve problem\".<br>В таблице появятся значения в квадратных скобках которые обозначают количество<br>"
                    + "        перевезенного груза из соответствующего производителя к потребителю.<br>Общая стоимость наиболее отимального плана будет отображена под надписью"
                    + "        с типом задачи."
                    + "    </p>"
                    + "</html>", "Reference", JOptionPane.NO_OPTION);
        });
        this.chooseFileMenuItem.addActionListener((ActionEvent e) -> {
            JFileChooser fileChooser = new JFileChooser();
            int result = fileChooser.showOpenDialog(null);
            if (result == JFileChooser.APPROVE_OPTION) {
                // user selected a file
                File selectedFile = fileChooser.getSelectedFile();
                readFile(selectedFile);
                initTask();
                renderTableGUI();
                buttonSolve.setEnabled(true);
            }
        });
        this.buttonSolve.addActionListener((ActionEvent e) -> {
            solveProblem();
        });
    }

    public void initTask() {
        if (totalProd > totalCon) {
            // over production
            // add new consumer with 0 tariffs
            for (int i = 0; i < producerPowers.size(); ++i) {
                cells.add(new Cell(i, consumerPowers.size(), 0));
            }
            consumerPowers.add(totalProd - totalCon);
            this.taskTypeLabel.setText("task type: Over production");
        } else if (totalProd < totalCon) {
            // over consuming
            for (int i = 0; i < consumerPowers.size(); ++i) {
                cells.add(new Cell(producerPowers.size(), i, 0));
            }
            producerPowers.add(totalCon - totalProd);
            this.taskTypeLabel.setText("task type: Over consuming");
        } else {
            // normal situation
            this.taskTypeLabel.setText("task type: Default");
        }
    }

    public void renderTableGUI() {
        tableData = new Object[producerPowers.size()][consumerPowers.size() + 1];
        cells.forEach(cell -> tableData[cell.producer][cell.consumer + 1] = cell.tariff);
        cells.stream().filter(c -> c.usedAmount > 0).forEach(c -> tableData[c.producer][c.consumer + 1] += "[" + c.usedAmount + "]");
        for (int i = 0; i < producerPowers.size(); ++i) {
            tableData[i][0] = "P" + (i + 1) + " " + producerPowers.get(i);
        }
        String[] columnHeader = new String[consumerPowers.size() + 1];
        for (int i = 1; i < columnHeader.length; ++i) {
            columnHeader[i] = "C" + i + " " + consumerPowers.get(i - 1);
        }
        columnHeader[0] = "";
        this.tariffTable.setModel(new DefaultTableModel(
                tableData,
                columnHeader
        ));
    }

    public int getNumberOfConsumers(String line) {
        return line.split("\\s+").length - 1;
    }

    public void readFile(File file) {
        try {
            BufferedReader reader = new BufferedReader(new FileReader(file));
            String line;
            ArrayList<String> lines = new ArrayList<>();
            while ((line = reader.readLine()) != null) {
                lines.add(line);
            }
            cells = new ArrayList<>();
            consumerPowers = new ArrayList<>(getNumberOfConsumers(lines.get(0)));
            producerPowers = new ArrayList<>(lines.size() - 1);
            String[] tokens;
            // now process each line, remebering about special last line
            for (int i = 0; i < lines.size() - 1; ++i) {
                tokens = lines.get(i).split("\\s+");
                for (int j = 0; j < tokens.length - 1; ++j) {
                    cells.add(new Cell(i, j, Double.parseDouble(tokens[j])));
                }
                double value = Double.parseDouble(tokens[tokens.length - 1]);
                totalProd += value;
                producerPowers.add(i, value);
            }
            tokens = lines.get(lines.size() - 1).split("\\s+");
            for (int i = 0; i < tokens.length; ++i) {
                double value = Double.parseDouble(tokens[i]);
                totalCon += value;
                consumerPowers.add(i, value);
            }
        } catch (IOException ex) {
            JOptionPane.showMessageDialog(null, "Error reading file", "Error", JOptionPane.ERROR_MESSAGE);
        }
    }

    public boolean any(Double[] arr) {
        for (int i = 0; i < arr.length; ++i) {
            if (arr[i] != 0) {
                return true;
            }
        }
        return false;
    }

    public void solveProblem() {
        // sort cells so the first one is with the lowest tariff
        Collections.sort(cells);
        int currentCell = 0;
        // this will keep the number of products that has to be sent to each consumer
        Double[] tmpConsumer = consumerPowers.toArray(new Double[consumerPowers.size()]);
        Double[] tmpProducer = producerPowers.toArray(new Double[producerPowers.size()]);
        while (any(tmpConsumer) && any(tmpProducer)) {
            int consumer = cells.get(currentCell).consumer;
            int producer = cells.get(currentCell).producer;
            // if some of these is false that means it's used completely and cannot be used again
            if (tmpConsumer[consumer] > 0 && tmpProducer[producer] > 0) {
                // reduce number of products that have to be sent to this consumer
                double delivery = Math.min(tmpConsumer[consumer], tmpProducer[producer]);
                tmpConsumer[consumer] -= delivery;
                // reduce remained number of goods of the consumer
                tmpProducer[producer] -= delivery;
                cells.get(currentCell).usedAmount += delivery;
            }
            ++currentCell;
        }
        // after the solution is found, check it
        while (true) {
            double total = 0;
            ArrayList<Cell> usedCells = cells.stream().filter(cell -> cell.usedAmount > 0).collect(Collectors.toCollection(ArrayList::new));
            consValues = new double[consumerPowers.size()];
            prodValues = new double[producerPowers.size()];
            Arrays.fill(consValues, unusedValue);
            Arrays.fill(prodValues, unusedValue);
            consValues[usedCells.get(0).consumer] = 0;
            prodValues[usedCells.get(0).producer] = usedCells.get(0).tariff;
            substitute(0, usedCells);
            Cell minSumCell = getMinSumCell();
            if (minSumCell.tariff >= 0) {
                for (int i = 0; i < usedCells.size(); ++i) {
                    total += usedCells.get(i).tariff * usedCells.get(i).usedAmount;
                }
                labelTotal.setText("Total: " + total);
                renderTableGUI();
                break;
            } else {
                minSumCell.flag = PLUS;
                dfs(copyList(cells), minSumCell, HORIZONTAL, new ArrayList<>());
                if (optimizedPath == null) {
                    dfs(copyList(cells), minSumCell, VERTICAL, new ArrayList<>());
                }
                double maxNegative = -1e9;
                for (int i = 0; i < optimizedPath.size(); ++i) {
                    if (optimizedPath.get(i).flag == MINUS && optimizedPath.get(i).usedAmount > 0) {
                        if (-1 * optimizedPath.get(i).usedAmount > maxNegative) {
                            maxNegative = -1 * optimizedPath.get(i).usedAmount;
                        }
                    }
                }
                maxNegative *= -1;
                for (int i = 0; i < optimizedPath.size(); ++i) {
                    int cell = cells.indexOf(optimizedPath.get(i));
                    if (optimizedPath.get(i).flag == PLUS) {
                        cells.get(cell).usedAmount += maxNegative;
                    }
                    if (optimizedPath.get(i).flag == MINUS) {
                        cells.get(cell).usedAmount -= maxNegative;
                    }
                }
                cells.forEach(cell -> {
                    cell.used = false;
                    cell.flag = 0;
                });
            }
        }
    }

    ArrayList<Cell> copyList(ArrayList<Cell> list) {
        ArrayList<Cell> copyList = new ArrayList<>(list.size());
        list.forEach(cell -> copyList.add(new Cell(cell)));
        return copyList;
    }

    public boolean validPath(ArrayList<Cell> path) {
        for (int i = 0; i < path.size() - 1; ++i) {
            if (path.get(i + 1).consumer != path.get(i).consumer
                    && path.get(i + 1).producer != path.get(i).producer) {
                return false;
            }
        }
        int[] consCount = new int[consumerPowers.size()];
        int[] prodCount = new int[producerPowers.size()];
        path.forEach(cell -> {
            consCount[cell.consumer]++;
            prodCount[cell.producer]++;
        });
        for (int i = 0; i < consCount.length; ++i) {
            if (consCount[i] % 2 == 1) {
                return false;
            }
        }
        for (int i = 0; i < prodCount.length; ++i) {
            if (prodCount[i] % 2 == 1) {
                return false;
            }
        }
        return true;
    }

    public void dfs(ArrayList<Cell> tariffsCells, Cell currentCell, int scanDirection, ArrayList<Cell> path) {
        int c = currentCell.consumer;
        int p = currentCell.producer;
        path.add(currentCell);
        if (scanDirection == HORIZONTAL) {
            tariffsCells.stream().filter(cell -> cell.producer == p && cell.canBeFlagged(currentCell)).forEach(cell -> {
                cell.flag = currentCell.flag == MINUS ? PLUS : MINUS;
                cell.used = true;
                dfs(tariffsCells, cell, VERTICAL, copyList(path));
            });
        } else if (scanDirection == VERTICAL) {
            tariffsCells.stream().filter(cell -> cell.consumer == c && cell.canBeFlagged(currentCell)).forEach(cell -> {
                cell.flag = currentCell.flag == MINUS ? PLUS : MINUS;
                cell.used = true;
                dfs(tariffsCells, cell, HORIZONTAL, copyList(path));
            });
        }
        if (validPath(copyList(path))) {
            this.optimizedPath = copyList(path);
        }
    }

    public void substitute(int currentCell, ArrayList<Cell> usedCells) {
        // assumption - both consValue and prodValue is already initialized for 
        // these producer and consumer
        int consumer = usedCells.get(currentCell).consumer;
        int producer = usedCells.get(currentCell).producer;
        for (int i = 0; i < usedCells.size(); ++i) {
            if (i != currentCell) {
                if (usedCells.get(i).producer == producer) {
                    int cellConsumer = usedCells.get(i).consumer;
                    // check if the consumer isn't initialized yet
                    if (consValues[cellConsumer] == unusedValue) {
                        consValues[cellConsumer] = usedCells.get(i).tariff - prodValues[producer];
                        substitute(i, usedCells);
                    }
                }
                if (usedCells.get(i).consumer == consumer) {
                    int cellProducer = usedCells.get(i).producer;
                    if (prodValues[cellProducer] == unusedValue) {
                        prodValues[cellProducer] = usedCells.get(i).tariff - consValues[consumer];
                        substitute(i, usedCells);
                    }
                }
            }
        }
    }

    public Cell getMinSumCell() {
        Cell minCell = new Cell(0, 0, unusedValue);
        for (int i = 0; i < cells.size(); ++i) {
            Cell current = cells.get(i);
            if (current.tariff - (consValues[current.consumer] + prodValues[current.producer]) < minCell.tariff) {
                minCell.tariff = current.tariff - (consValues[current.consumer] + prodValues[current.producer]);
                minCell.producer = current.producer;
                minCell.consumer = current.consumer;
            }
        }
        return minCell;
    }

    public static void main(String args[]) {
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new Window().setVisible(true);
            }
        });
    }

    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jScrollPane1 = new javax.swing.JScrollPane();
        tariffTable = new javax.swing.JTable(){
            @Override
            public Component prepareRenderer(
                TableCellRenderer renderer, int row, int col) {
                if (col == 0) {
                    return this.getTableHeader().getDefaultRenderer()
                    .getTableCellRendererComponent(this,
                        this.getValueAt(row, col), false, false, row, col);
                } else {
                    return super.prepareRenderer(renderer, row, col);
                }
            }};
            buttonSolve = new javax.swing.JButton();
            taskTypeLabel = new javax.swing.JLabel();
            labelTotal = new javax.swing.JLabel();
            menuBar = new javax.swing.JMenuBar();
            fileMenu = new javax.swing.JMenu();

            setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

            tariffTable.setModel(new javax.swing.table.DefaultTableModel(
                new Object [][] {
                    {"Producer 1", null, null, null, null},
                    {"Producer 2", null, null, null, null},
                    {"Producer 3", null, null, null, null},
                    {"Producer 4", null, null, null, null}
                },
                new String [] {
                    "", "Consumer 1", "Consumer 2", "Consumer 3", "Consumer 4"
                }
            ));
            jScrollPane1.setViewportView(tariffTable);

            buttonSolve.setText("Solve problem");

            fileMenu.setText("File");

            chooseFileMenuItem = new javax.swing.JMenuItem("Choose file");
            fileMenu.add(chooseFileMenuItem);
            referenceMenuItem = new javax.swing.JMenuItem("Open help");
            fileMenu.add(referenceMenuItem);

            menuBar.add(fileMenu);

            setJMenuBar(menuBar);

            javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
            getContentPane().setLayout(layout);
            layout.setHorizontalGroup(
                layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                .addGroup(layout.createSequentialGroup()
                    .addContainerGap()
                    .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                    .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addComponent(buttonSolve)
                        .addComponent(taskTypeLabel)
                        .addComponent(labelTotal))
                    .addContainerGap(113, Short.MAX_VALUE))
            );
            layout.setVerticalGroup(
                layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                    .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addGroup(layout.createSequentialGroup()
                            .addComponent(buttonSolve)
                            .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                            .addComponent(taskTypeLabel)
                            .addGap(55, 55, 55)
                            .addComponent(labelTotal))
                        .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addContainerGap())
            );

            buttonSolve.setEnabled(false);

            pack();
        }// </editor-fold>//GEN-END:initComponents

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton buttonSolve;
    private javax.swing.JMenu fileMenu;
    private javax.swing.JMenuItem chooseFileMenuItem;
    private javax.swing.JMenuItem referenceMenuItem;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JLabel labelTotal;
    private javax.swing.JMenuBar menuBar;
    private javax.swing.JTable tariffTable;
    private javax.swing.JLabel taskTypeLabel;
    // End of variables declaration//GEN-END:variables
}
