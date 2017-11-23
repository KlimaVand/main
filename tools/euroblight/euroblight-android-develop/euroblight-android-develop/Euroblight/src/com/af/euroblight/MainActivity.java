package com.af.euroblight;

import java.util.ArrayList;
import java.util.Arrays;

import com.af.euroblight.adapters.SubviewsListAdapter;
import com.af.euroblight.managers.DataManager;
import com.af.euroblight.AppDelegate.SUBVIEWS_DEFINITION;

import android.net.Uri;
import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ListView;

public class MainActivity extends Activity implements OnItemClickListener {
	
	// Variables
	private ListView listView;
	private SubviewsListAdapter subviewListAdapter;
	
	/** Override Methods */
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		listView = (ListView)findViewById(R.id.listOfSubviews);
		configureListView();
	}
	
	@Override
	protected void onResume() {
		super.onResume();
		if (!DataManager.sharedManager().isStoredUserCredentialsSufficent()) { 
			openUserCredentialsActivity();
			return;
		}
		subviewListAdapter.notifyDataSetChanged();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
	    switch(item.getItemId()) {
	    case R.id.menu_action_login_settings:
	        openUserCredentialsActivity();
	    	break;
	    case R.id.menu_action_help:
	    	openHelpWebPage();
	    	break;
	    default:
	        break;
	    }
	    return true;
	}

	@Override
	public void onItemClick(AdapterView<?> adapter, View view, int position, long id) {
		 this.executeOpenSubview(SUBVIEWS_DEFINITION.values()[position]);
	}
	
	private void configureListView() {
		SUBVIEWS_DEFINITION[] subviews = AppDelegate.getDelegate().getSubviews();
		ArrayList<SUBVIEWS_DEFINITION> subviewsKeyList = 
				new ArrayList<SUBVIEWS_DEFINITION>(Arrays.asList(subviews));
		subviewListAdapter = new SubviewsListAdapter(this, 
				R.layout.activity_main_list_row, subviewsKeyList); 
		listView.setAdapter(subviewListAdapter);
		listView.setOnItemClickListener(this);
	}
	
	/** Private Methods */
	
	private void executeOpenSubview(SUBVIEWS_DEFINITION subviewDefinition) {
		Intent intent = new Intent();
		switch (subviewDefinition) {
		case REPORT:
			intent.setClass(MainActivity.this, ReportActivity.class);
			break;
		case MAP:
			intent.setClass(MainActivity.this, MapActivity.class);
			break;
		case PRESSURE:
			intent.setClass(MainActivity.this, PressureActivity.class);
			break;
		case ERROR:
			intent.setClass(MainActivity.this, UnsentReportsActivity.class);
			break;
		default:
			return;	
		}
		MainActivity.this.startActivity(intent);
	}
	
	private void openUserCredentialsActivity() {
        Intent intent = new Intent();
        intent.setClass(MainActivity.this, UserCredentialsActivity.class);
        MainActivity.this.startActivity(intent);
	}
	
	private void openHelpWebPage() {
		Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse("http://euroblight.net"));
		startActivity(browserIntent);
	}
}