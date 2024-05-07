import java.util.ArrayList;

public class Process {
	 private int idOfProcess;
	 private double start=-1;
	 private double finish=-1;
	 private double duration ;
	 private double remainingTime ;
	 private int size;
	 private int locationOfPage; 
	 private double waitingTime=0;
	 private double turningAroundTime=0;
	 private double timeOfArrival=0;
	 private int hit = 0;
	 private int fault = 0;
	 private Thread thread;
     ArrayList<TableData> pageTable = new ArrayList();
     public Process(int idOfProcess , double timeOfArrival,double duration ,int size){
        this.idOfProcess = idOfProcess; 
        this.duration=duration;
        this.size=size;
        this.remainingTime = duration;
        this.timeOfArrival = timeOfArrival;
        this.thread = new Thread(String.valueOf(idOfProcess));
        this.thread.setName("Process " + idOfProcess + " own Thread");

    }   

    Process() {
    	
    }
    
	public Thread getThread() {
		return thread;
	}

	public int getIdOfProcess() {
		return idOfProcess;
	}

	public void setIdOfProcess(int idOfProcess) {
		this.idOfProcess = idOfProcess;
	}

	public double getStart() {
		return start;
	}

	public void setStart(double start) {
		this.start = start;
	}

	public double getFinish() {
		return finish;
	}

	public void setFinish(double finish) {
		this.finish = finish;
	}

	public double getDuration() {
		return duration;
	}

	public void setDuration(double duration) {
		this.duration = duration;
	}

	public double getRemainingTime() {
		return remainingTime;
	}

	public void setRemainingTime(double remainingTime) {
		this.remainingTime = remainingTime;
	}

	public int getSize() {
		return size;
	}

	public void setSize(int size) {
		this.size = size;
	}

	public int getLocationOfPage() {
		return locationOfPage;
	}

	public void setLocationOfPage(int locationOfPage) {
		this.locationOfPage = locationOfPage;
	}

	public double getWaitingTime() {
		return waitingTime;
	}

	public void setWaitingTime(double waitingTime) {
		this.waitingTime = waitingTime;
	}

	public double getTurningAroundTime() {
		return turningAroundTime;
	}

	public void setTurningAroundTime(double turningAroundTime) {
		this.turningAroundTime = turningAroundTime;
	}

	public double getTimeOfArrival() {
		return timeOfArrival;
	}

	public void setTimeOfArrival(double timeOfArrival) {
		this.timeOfArrival = timeOfArrival;
	}

	public int getHit() {
		return hit;
	}

	public void setHit(int hit) {
		this.hit = hit;
	}

	public int getFault() {
		return fault;
	}

	public void setFault(int fault) {
		this.fault = fault;
	}

	public ArrayList<TableData> getPageTable() {
		return pageTable;
	}

	public void setPageTable(ArrayList<TableData> pageTable) {
		this.pageTable = pageTable;
	}
    
}
