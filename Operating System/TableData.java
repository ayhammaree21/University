public class TableData {
	  
	   private int idOfProcess ;
	   private int numberOfPage;
	   
	   public TableData(int idOfProcess ,int numberOfPage ) {
		   this.idOfProcess=idOfProcess;
		   this.numberOfPage=numberOfPage;
	   }

	   public TableData(){
		   
	   }

	public int getIdOfProcess() {
		return idOfProcess;
	}

	public void setIdOfProcess(int idOfProcess) {
		this.idOfProcess = idOfProcess;
	}

	public int getNumberOfPage() {
		return numberOfPage;
	}

	public void setNumberOfPage(int numberOfPage) {
		this.numberOfPage = numberOfPage;
	}
	   
}
