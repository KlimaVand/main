package com.af.euroblight2.adapters;

import java.util.List;

import com.af.euroblight2.R;
import com.af.euroblight2.objects.Report;

import android.content.Context;
import android.graphics.Bitmap;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.TextView;

public class UnsentReportsListAdapter extends ArrayAdapter<Report> {
	
	public UnsentReportsListAdapter(Context context, 
									int rowViewResourceId,
									List<Report> objects) {
		super(context, rowViewResourceId, objects);
	}
	
	public View getView(int position, View convertView, ViewGroup parent) {
		LayoutInflater layoutInflater = (LayoutInflater)super.getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		View unsetReportRowView = layoutInflater.inflate(R.layout.activity_unsent_reports_list_row, parent, false);
		ImageView imageViewForUnsentReport = (ImageView)unsetReportRowView.findViewById(R.id.row_unsent_report_photo);
		TextView textViewForUnsentReport = (TextView)unsetReportRowView.findViewById(R.id.row_unsent_report_date);
		Report unsentReport = this.getItem(position);
		if (unsentReport.getPhoto() != null) {
			Bitmap imageBitmap = unsentReport.getPhoto().getBitmap();
			if (imageBitmap != null) {
				imageViewForUnsentReport.setImageBitmap(imageBitmap);
			}
		}
		textViewForUnsentReport.setText(String.format("%s", unsentReport.getCreatedDate()));
		return unsetReportRowView;	
	}
}