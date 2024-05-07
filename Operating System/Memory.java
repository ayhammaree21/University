public class Memory {
	
	   private TableData idOfPage;
	   private boolean ReferenceBitValidation;
	
	   public Memory() {
		   ReferenceBitValidation = false;
	}
	   
	 

	public Memory(TableData idOfPage, boolean referenceBitValidation) {
		super();
		this.idOfPage = idOfPage;
		ReferenceBitValidation = referenceBitValidation;
	}



	public TableData getIdOfPage() {
		return idOfPage;
	}

	public void setIdOfPage(TableData idOfPage) {
		this.idOfPage = idOfPage;
	}

	public boolean isReferenceBitValidation() {
		return ReferenceBitValidation;
	}

	public void setReferenceBitValidation(boolean referenceBitValidation) {
		ReferenceBitValidation = referenceBitValidation;
	}
	    
}
