package com.af.euroblight2.adapters;

import java.util.List;

import com.af.euroblight2.R;
import com.af.euroblight2.managers.DataManager;
import com.af.euroblight2.AppDelegate;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.TextView;

public class SubviewsListAdapter extends ArrayAdapter<AppDelegate.SUBVIEWS_DEFINITION> {

	public SubviewsListAdapter(Context context, int rowViewResourceId,
			List<AppDelegate.SUBVIEWS_DEFINITION> objects) {
		super(context, rowViewResourceId, objects);
	}
		
	public View getView(int position, View convertView, ViewGroup parent) {
		LayoutInflater layoutInflater = (LayoutInflater)super.getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		View subviewRowView = layoutInflater.inflate(R.layout.activity_main_list_row, parent, false);
		ImageView imageViewForSubview = (ImageView)subviewRowView.findViewById(R.id.row_subview_icon);
		TextView textViewForSubview = (TextView)subviewRowView.findViewById(R.id.row_subview_text);
		switch (AppDelegate.SUBVIEWS_DEFINITION.values()[position]) {
		case REPORT:
			imageViewForSubview.setImageResource(R.drawable.report_icon);
			textViewForSubview.setText(R.string.title_activity_report);
			break;
		case MAP:
			imageViewForSubview.setImageResource(R.drawable.map_icon);
			textViewForSubview.setText(R.string.title_activity_map);
			break;
		case PRESSURE:
			imageViewForSubview.setImageResource(R.drawable.pressure_icon);
			textViewForSubview.setText(R.string.title_activity_pressure);
			break;
		case ERROR:
			imageViewForSubview.setImageResource(R.drawable.alert_icon);
			// TODO: Change to activity title
			int unsentReports = DataManager.sharedManager().storedReports().size();
			String unsentReportsTitle = getContext().getString(R.string.title_activity_unsent_reports);
			textViewForSubview.setText(String.format("%s (%d)", unsentReportsTitle, unsentReports));
			break;
		default:
			break;
		}
		return subviewRowView;	
	}
}