package com.af.euroblight2;

import android.net.Uri;
import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;

import com.af.euroblight2.R;
import com.af.euroblight2.managers.DataManager;

public class UserCredentialsActivity extends Activity {

	// GUI Variables	
	Button buttonSave;
	EditText editTextUsername;
	EditText editTextPassword;
	LinearLayout layoutUsername;
	LinearLayout layoutPassword;
	
	// Variables
	String currentUsernameInput;
	String currentPasswordInput;
	boolean isUsernameInputValid;
	boolean isPasswordInputValid;
	
	/** Override Methods */
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_user_credentials);
		buttonSave = (Button)findViewById(R.id.buttonSave);
		editTextUsername = (EditText)findViewById(R.id.editTextUsername);
		editTextPassword = (EditText)findViewById(R.id.editTextPassword);
		layoutUsername = (LinearLayout)findViewById(R.id.user_credentials_layout_username);
		layoutPassword = (LinearLayout)findViewById(R.id.user_credentials_layout_password); 
		currentUsernameInput = DataManager.sharedManager().storedUsername();
		currentPasswordInput = DataManager.sharedManager().storedPassword();
		editTextUsername.setText(currentUsernameInput); 
		editTextPassword.setText(currentPasswordInput);
		isUsernameInputValid = true;
		isPasswordInputValid = true;
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		int menuId = 
				(DataManager.sharedManager().isStoredUserCredentialsSufficent()) ? 
						R.menu.user_credentials : R.menu.user_credentials_start;
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(menuId, menu);
		return true;
	}
	
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case R.id.menu_action_close:
			finish();
			break;
		case R.id.menu_action_help:
			openHelpWebPage();
			break;
		}
		return super.onOptionsItemSelected(item);
	}	
	
	@Override
	public void onBackPressed() {
		// disable back.
	}
	
	/** Public Methods */
	
	public void saveButtonTapped(View sender) {
		currentUsernameInput =  editTextUsername.getText().toString();
		currentPasswordInput = editTextPassword.getText().toString();
		isUsernameInputValid = validateUsernameString(currentUsernameInput);
		isPasswordInputValid = validatePasswordString(currentPasswordInput);
		updateInputsValidationColor();
		if (isUsernameInputValid && isPasswordInputValid) {
			DataManager.sharedManager().storeUserCredentials(currentUsernameInput, currentPasswordInput);
			finish();
		}
	}
	
	/** Private Methods */
	
	private boolean validateUsernameString(String username) {
		return !username.isEmpty();
	}
	
	private boolean validatePasswordString(String password) {
		return !password.isEmpty();
	}
	
	private void updateInputsValidationColor() {
		layoutUsername.setBackgroundResource(
				(isUsernameInputValid) ? R.color.cell_default_input_color : 
									  R.color.cell_invalid_input_color);
		layoutPassword.setBackgroundResource(
				(isPasswordInputValid) ? R.color.cell_default_input_color : 
									  	 R.color.cell_invalid_input_color);
	}
	
	private void openHelpWebPage() {
		Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse("http://euroblight.net"));
		startActivity(browserIntent);
	}
}