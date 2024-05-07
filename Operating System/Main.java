import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
import javax.swing.JOptionPane;
import javafx.application.Application;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.image.Image;
import javafx.stage.Stage;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.*;
import javafx.stage.FileChooser;
import java.io.File;
import java.net.URL;
import java.util.ResourceBundle;
import javafx.scene.control.ToggleGroup;
import javafx.scene.control.cell.PropertyValueFactory;

public class Main extends Application implements Initializable{
	
	public ObservableList<ResultTable> List = FXCollections.observableArrayList();
	public ObservableList<MemoryTable> List2 = FXCollections.observableArrayList();
	
	  @FXML
	    private TextField textFieldPathFile;

	    @FXML
	    private Spinner<Integer> spinnerTimeQuantaum=new Spinner<Integer>();

	    @FXML
	    private Spinner<Integer> spinnerMemorySize=new Spinner<Integer>();

	    @FXML
	    private RadioButton radioBtnSecondChance;

	    @FXML
	    private RadioButton radioBtnClock;

	    @FXML
	    private TableView<ResultTable> tabelView1;

	    @FXML
	    private TableColumn<ResultTable, Integer> pId;

	    @FXML
	    private TableColumn<ResultTable, Integer> numberOfFaults;

	    @FXML
	    private TableColumn<ResultTable, Double> start;

	    @FXML
	    private TableColumn<ResultTable, Double> finish;

	    @FXML
	    private TableColumn<ResultTable, Double> turnAround;

	    @FXML
	    private TableColumn<ResultTable, Double> waitTime;

	    @FXML
	    private TableView<MemoryTable> tabelView2;

	    @FXML
	    private TableColumn<MemoryTable, Integer> process_ID;

	    @FXML
	    private TableColumn<MemoryTable, Integer> page_ID;

	    @FXML
	    private TableColumn<MemoryTable, Boolean> trueFalse;

	    @FXML
	    private TextField textFieldTotalCycles;

	    @FXML
	    private TextField textFieldTotalFaults;

public static int N;
public static int M;
public static int S;
public int numberOfpages;
public int numberOfFrames;
public static int timeQuatum=2;
public int memoryPointer;
public int pageReferenceSize;
static ArrayList<Process> systemProcessArray = new ArrayList();
static ArrayList<Process> queueOfReadyStatus = new ArrayList();
static ArrayList<Memory> memory_frames = new ArrayList();
static ArrayList<Process> queueOfBlockStatus = new ArrayList();
static ArrayList<Thread> threadList = new ArrayList();
static Lock lock = new ReentrantLock();
//static Memory memory_frames[];
static int memorySize = 10;
final static int sequencesOfDisk = 300;
final static int sequencesOfMAU = 1;
static int memoryUsed = 0;
static int pointer = 0;
static int sequences = 0;
static int formerProcess = 0;
static int faultsCount = 0;
public static Stage stage ;
public static String URLPath="";
static int algorithmselect=0;

@Override
public void start(Stage primaryStage) throws Exception{
   
   Parent root = FXMLLoader.load(getClass().getResource("sample.fxml"));
   stage = primaryStage ;
   primaryStage.setTitle("Virtual Memory Mangement");
   primaryStage.getIcons().add(new Image(getClass().getResourceAsStream("/Images/operating.png")));
  
  primaryStage.setScene(new Scene(root));
   primaryStage.setResizable(false);
  
    primaryStage.show();
    primaryStage.centerOnScreen();
}
public static void readTrace(String trace,int pid) {
	   
	trace = trace.replaceAll("[\n \t]", "");
    
      String []slice = trace.split(",");
      
	   for(String t:slice) {
           
           int numberOfPage = (int) (Integer.parseInt(t.trim(), 16));
           numberOfPage = (int) (numberOfPage / Math.pow(2, 12));
           systemProcessArray.get(systemProcessArray.size() - 1).pageTable.add(new TableData(pid,numberOfPage));
    
	   }
}


public static void fileReading(File fName) throws InterruptedException {

    Scanner userText = null;
    
        try {
        	userText = new Scanner(fName);
		} catch (FileNotFoundException e) {// abj
			e.printStackTrace();
		}
        N= Integer.valueOf( userText.nextLine());
        M=Integer.valueOf(userText.nextLine());
        S=Integer.valueOf(userText.nextLine());

        userText.nextLine();
        userText.nextLine();
        while (userText.hasNextLine()) {
            String userLine = userText.nextLine();
            userLine = userLine.replaceAll("[\n \t]", "");
            if (userLine.equals("")) {
                continue;
            }
            String[] userSlice;
            userSlice = userLine.split("[|]");
            systemProcessArray.add(new Process(Integer.parseInt(userSlice[0]), Double.parseDouble(userSlice[1]), Double.parseDouble(userSlice[2]), Integer.parseInt(userSlice[3])));
            readTrace(userSlice[4],Integer.parseInt(userSlice[0]));
            
        }

        Runnable roundRobinRunnable = () -> {
            try {
				roundRobinAlgorithm();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
        };
        Thread roundRobinThread = new Thread(roundRobinRunnable);
        roundRobinThread.setName("Round Robin Thread");
        threadList.add(roundRobinThread);
        roundRobinThread.start();
        roundRobinThread.join();
        for (int j = 0; j < systemProcessArray.size(); j++) {
            faultsCount += systemProcessArray.get(j).getFault();
          
        }
        

}
public static boolean processFinishingStatusChecked() {
    for (Process p : systemProcessArray) {
        if (p.getFinish() == -1) 
        {
            return false;
            
        }
    }
    return true;
}

public static void checkProcessesArrival(int time) {
    for (Process c : systemProcessArray) {
        if (!queueOfReadyStatus.contains(c) && c.getTimeOfArrival() == time)
        {
            queueOfReadyStatus.add(c);
        }
    }
}
/////////////////////////////////////////////////////////////////////


public static int referenceBitResetingFunction(int arrow) {
    for (int i = arrow; i < memorySize; i +=1 % memorySize) {

        if (memory_frames.get(i).isReferenceBitValidation()) {
        	arrow ++;
        	arrow %= memorySize;
        	memory_frames.get(i).setReferenceBitValidation(false);
        } else {
            break;
        }
    }
    return arrow;
}
///////////////////////////////////////////////////////////////////////////////
public static boolean pageEqualization(TableData firstPage, TableData secondPage) {
    if (secondPage == null || firstPage == null) return false;
    if ((firstPage.getIdOfProcess() == secondPage.getIdOfProcess()) && (firstPage.getNumberOfPage() == secondPage.getNumberOfPage())) return true;

    return false;
}
////////////////////////////////////////////////////////////////////////////////

public static int memoryLocation(TableData page) {
	int sad = 0;
    while(sad < memorySize) {
        if (pageEqualization(page, memory_frames.get(sad).getIdOfPage())) return sad;
        sad++;
    }
    return -1;
}
////////////////////////////////////////////////////////////////////////////////



public static void clockAlgorithmFunction(TableData page){
	lock.lock();
    int memoryPageIndex = memoryLocation(page);
    if (memorySize > memoryUsed) {
        if (-1 != memoryPageIndex) {
        	sequences = sequences + sequencesOfMAU;
            memory_frames.get(memoryPageIndex).setReferenceBitValidation(true);
        } else {
        	queueOfBlockStatus.add(queueOfReadyStatus.get(0));
        	
        	memory_frames.get(pointer).setIdOfPage(page);
        	memory_frames.get(pointer).setReferenceBitValidation(true);
        	memoryUsed= memoryUsed + 1;
        	pointer ++;
        	pointer %=  memorySize;
            systemProcessArray.get(page.getIdOfProcess()).setFault(systemProcessArray.get(page.getIdOfProcess()).getFault()+1);
        	sequences = sequences + sequencesOfDisk;
        	
        	queueOfBlockStatus.remove(0);
        }

    } else if (-1 != memoryPageIndex) {
    	sequences = sequences + sequencesOfMAU;
    	memory_frames.get(memoryPageIndex).setReferenceBitValidation(true);
        
    } else {
    	queueOfBlockStatus.add(queueOfReadyStatus.get(0));
    	

        if (memory_frames.get(pointer).isReferenceBitValidation() == true) pointer = referenceBitResetingFunction(pointer);
        
        systemProcessArray.get(page.getIdOfProcess()).setFault(systemProcessArray.get(page.getIdOfProcess()).getFault()+1);
        memory_frames.get(pointer).setIdOfPage(page);
        memory_frames.get(pointer).setReferenceBitValidation(true);
        pointer ++;
        pointer %= memorySize;
        sequences = sequences + sequencesOfDisk;
        queueOfBlockStatus.remove(0);
    }
lock.unlock();
}

public static void secondChanceAlgorithmFunction(TableData page){
	lock.lock();
	int memoryPageIndex = memoryLocation(page);
    if (memorySize > memoryUsed) {
        if (-1 != memoryPageIndex) {
        	sequences = sequences + sequencesOfMAU;
            memory_frames.get(memoryPageIndex).setReferenceBitValidation(true);
        } else {
        	queueOfBlockStatus.add(queueOfReadyStatus.get(0));
        	
        	
           	queueOfBlockStatus.add(systemProcessArray.get(page.getIdOfProcess()));
        	memory_frames.get(pointer).setIdOfPage(page);
        	memory_frames.get(pointer).setReferenceBitValidation(true);
        	memoryUsed= memoryUsed + 1;
        	pointer ++;
        	pointer %=  memorySize;
            systemProcessArray.get(page.getIdOfProcess()).setFault(systemProcessArray.get(page.getIdOfProcess()).getFault()+1);
        	sequences = sequences + sequencesOfDisk;
        	
            queueOfBlockStatus.remove(0);
        }

    } else if (-1 != memoryPageIndex) {
    	sequences = sequences + sequencesOfMAU;
    	memory_frames.get(memoryPageIndex).setReferenceBitValidation(true);
        
    } else {
    	queueOfBlockStatus.add(queueOfReadyStatus.get(0));
    	
    	while(true) {
    		if(memory_frames.get(0).isReferenceBitValidation()) {
    			memory_frames.add(new Memory(memory_frames.get(0).getIdOfPage(), false));
    			memory_frames.remove(0);
    			
    		} else {
    			systemProcessArray.get(page.getIdOfProcess()).setFault(systemProcessArray.get(page.getIdOfProcess()).getFault()+1);
    			memory_frames.set(0, new Memory(page,true));
    			 sequences = sequences + sequencesOfDisk;
    			
    		     queueOfBlockStatus.remove(0);
    			break;
    		}
    	}

    }
    lock.unlock();
}

public static void roundRobinAlgorithm() throws InterruptedException {
	lock.lock();
    int current = 0, j = 0, sad = 0;

    while(sad < memorySize)
    {
    	memory_frames.add(new Memory());
    	sad++;
    }
    
    lock.unlock();
    while (processFinishingStatusChecked() == false) {
        formerProcess = j;
        checkProcessesArrival(current);
        if (queueOfReadyStatus.size() == 0) {
        	current++;
            continue;
        }
        j %= queueOfReadyStatus.size();

        Process prc;
        prc = queueOfReadyStatus.get(j++);
        threadList.add(prc.getThread());
        
        if (formerProcess != j) sequences = sequences + 5;

        if (prc.getStart() == -1) prc.setStart(current);
        
        for (int i = prc.getLocationOfPage(); i < prc.getLocationOfPage() + timeQuatum; i++) {
        	TableData x=prc.pageTable.get(i);
        	
        	if(algorithmselect==2) {
        		Runnable clockRunnable = () -> {
        			clockAlgorithmFunction(x);
                };

        		Thread clockThread = new Thread(clockRunnable);
        		clockThread.setName("Clock Algorithm Thread");
        		threadList.add(clockThread);
        		clockThread.start();
        		clockThread.join();
        		
        	} else if(algorithmselect==1) {
        		Runnable secondChanceRunnable = () -> {
        			secondChanceAlgorithmFunction(x);
                };

        		Thread secondChanceThread = new Thread(secondChanceRunnable);
        		secondChanceThread.setName("Second Chance Algorithm Thread");
        		threadList.add(secondChanceThread);
        		secondChanceThread.start();
        		secondChanceThread.join();
        	}
            //printMemory();
            checkProcessesArrival(++current);
            prc.setRemainingTime(prc.getRemainingTime()-1);
            if (prc.getRemainingTime() == 0) break;
            
        }
        prc.setLocationOfPage(prc.getLocationOfPage() + timeQuatum);

        if (prc.getRemainingTime() == 0) {
        	prc.setFinish(current);
            queueOfReadyStatus.remove(prc);
            
            prc.setTurningAroundTime(prc.getFinish() - prc.getTimeOfArrival());
            prc.setWaitingTime(prc.getTurningAroundTime() - prc.getDuration());
        }

    }
    
}

public void printMemory() {
	List2.clear();
	   for (Memory m:memory_frames) {
		   if (m.getIdOfPage() != null ) {
			   MemoryTable t=new MemoryTable(String.valueOf(m.getIdOfPage().getIdOfProcess()),String.valueOf(m.getIdOfPage().getNumberOfPage()),m.isReferenceBitValidation());
			   List2.add(t);
		   }else {
			   MemoryTable t=new MemoryTable("x","x",false);
			   List2.add(t);
		   }
		   
	    }
	    
	   tabelView2.setItems(List2);
	}


public void printTableResult() {
	List.clear();
	for(Process p : systemProcessArray ) {
		ResultTable t = new ResultTable(p.getIdOfProcess(),p.getFault(),p.getTimeOfArrival(),p.getFinish(),p.getTurningAroundTime(),p.getWaitingTime());
		 List.add(t);
	}
	tabelView1.setItems(List);
}


@FXML
void btnBrowseOnAction(ActionEvent event) {

    FileChooser fileChooserShares = new FileChooser();
    fileChooserShares.setTitle("Select data file .txt");
    fileChooserShares.getExtensionFilters().addAll(
            new FileChooser.ExtensionFilter("Text Files", "*.txt"),

            new FileChooser.ExtensionFilter("TXT files (' txt')", "*.txt")

    );

    File selectedFile = fileChooserShares.showOpenDialog(Main.stage);
    if (String.valueOf(selectedFile).equals("null")) {
        return;
    }
    else{
        textFieldPathFile.setText(selectedFile.toString());
    }

}

@FXML
void btnReadOnAction(ActionEvent event) {
	
	try {
		if(textFieldPathFile.getText().equals("")) {
			  throw new Exception();
		}else {
			URLPath=textFieldPathFile.getText();
			
		}
	}catch (Exception e) {
        JOptionPane.showMessageDialog(null, "Please browse a file");

    }
	
}


@FXML
void btnRunOnAction(ActionEvent event) throws InterruptedException {
	boolean OK=true;
	timeQuatum= spinnerTimeQuantaum.getValue();
	memorySize=spinnerMemorySize.getValue();
	
	try {
		if(URLPath.equals("")) {
			  OK=false;
			  throw new Exception();
		}
		
	}catch (Exception e) {
        JOptionPane.showMessageDialog(null, "Please click read a file");

    }
	
	if(radioBtnSecondChance.isSelected()) {
		algorithmselect=1;
		List.clear();
		List2.clear();
		
		
	}else if(radioBtnClock.isSelected()) {
		algorithmselect=2;
		List.clear();
		List2.clear();
		
	}else {
		try {
			if(radioBtnSecondChance.isSelected()==false && radioBtnClock.isSelected()==false) {
				  OK=false;
				  throw new Exception();
			}
			
		}catch (Exception e) {
	        JOptionPane.showMessageDialog(null, "Please choose algorthim type ");

	    }
	}
	
	if(OK) {
		 File test = new File(URLPath);
		 systemProcessArray.clear();
		 memory_frames.clear();
		 threadList.clear();
			memoryUsed = 0;
			 sequences = 0;
			 pointer = 0;
			formerProcess = 0;
			 faultsCount = 0;
		    fileReading(test);
			printTableResult();
			printMemory();
			textFieldTotalFaults.setText(String.valueOf(faultsCount));
			textFieldTotalCycles.setText(String.valueOf(sequences));
			System.out.println("---------Our system thread synchronuization---------\n");
			 for(Thread th:threadList){
				 System.out.println("\t"+th.getName());
			 }
	}
   
	
}

@Override
public void initialize(URL location, ResourceBundle resources) {

    disableTextFiled();
    spinnerTimeQuantaum.setValueFactory(new SpinnerValueFactory.IntegerSpinnerValueFactory(1, 1000));
    spinnerMemorySize.setValueFactory(new SpinnerValueFactory.IntegerSpinnerValueFactory(1, 1000000));
    ToggleGroup group = new ToggleGroup();
    radioBtnSecondChance.setToggleGroup(group);
    radioBtnClock.setToggleGroup(group);
    
    pId.setCellValueFactory(new PropertyValueFactory<ResultTable, Integer>("PID"));
    numberOfFaults.setCellValueFactory(new PropertyValueFactory<ResultTable, Integer>("number_of_fault"));
    start.setCellValueFactory(new PropertyValueFactory<ResultTable, Double>("start"));
    finish.setCellValueFactory(new PropertyValueFactory<ResultTable, Double>("finish"));
   turnAround.setCellValueFactory(new PropertyValueFactory<ResultTable, Double>("turnAround"));
   waitTime.setCellValueFactory(new PropertyValueFactory<ResultTable, Double>("waitTime"));
   
   process_ID.setCellValueFactory(new PropertyValueFactory<MemoryTable, Integer>("process_ID"));
   page_ID.setCellValueFactory(new PropertyValueFactory<MemoryTable, Integer>("page_ID"));
   trueFalse.setCellValueFactory(new PropertyValueFactory<MemoryTable, Boolean>("trueFalse"));

}

private void disableTextFiled(){

    textFieldPathFile.setEditable(false);
    textFieldTotalFaults.setEditable(false);
    textFieldTotalCycles.setEditable(false);

}


public static void main(String[] args) throws FileNotFoundException, InterruptedException {
	launch(args);
	
    }

}





