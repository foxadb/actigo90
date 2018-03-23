import { Component, OnInit, ViewChild } from '@angular/core';
import { BaseChartDirective } from 'ng2-charts/ng2-charts';

import Price from '../models/price.model';

import { PriceService } from '../services/price.service';

@Component({
  selector: 'app-tracking',
  templateUrl: './tracking.component.html',
  styleUrls: ['./tracking.component.scss']
})
export class TrackingComponent implements OnInit {

  public trackingDate: Date;
  public trackingError: number;

  public prices: Array<Price>;

  @ViewChild('baseChart') chart: BaseChartDirective;

  public actigoPrices: Array<Number> = [];
  public hedgingPrices: Array<Number> = [];
  public chartData: Array<any> = [
    { data: this.actigoPrices, label: 'Actigo 90' },
    { data: this.hedgingPrices, label: 'Hedging Portfolio' }
  ];
  public chartLabels: Array<any> = [];

  public chartOptions: any = {
    responsive: true,
    elements: {
      point: {
        radius: 0
      }
    }
  };
  public chartLegend = true;
  public chartType = 'line';

  constructor(private priceService: PriceService) { }

  public ngOnInit(): void {
    // Price subscription
    this.priceService.getPrices(1, 5000).subscribe(
      prices => {
        this.prices = prices;
      },
      error => console.error('Error:', error),
      () => {
        // Sort spot by date
        this.prices.sort((a, b) =>
          new Date(a.date).getTime() - new Date(b.date).getTime());

        // Fill chart data
        this.prices.forEach(price => {
          this.chartLabels.push(new Date(price.date).toLocaleDateString());
          this.actigoPrices.push(price.actigo);
          this.hedgingPrices.push(price.hedging);
        });

        // Refresh chart
        this.chart.ngOnInit();
      }
    );
  }

  public computeTrackingError(): void {
    // Compute error serie and average error
    const errorsSerie = [];
    let averageError = 0;
    let i = 0;
    while (new Date(this.prices[i].date).getTime()
      <= new Date(this.trackingDate).getTime()) {
      const price = this.prices[i];
      const error = price.hedging - price.actigo;
      errorsSerie.push(error);
      averageError += error;
      ++i;
    }
    averageError /= i;

    // Compute tracking error (std dev)
    let trackingError = 0;
    errorsSerie.forEach(error => trackingError += error ** 2);
    trackingError /= i;
    trackingError -= averageError ** 2;
    trackingError = Math.sqrt(trackingError);

    // Tracking error in percent
    this.trackingError = trackingError * 100;
  }

  public chartClicked(event: any): void {
    console.log(event);
  }

  public chartHovered(event: any): void {
    console.log(event);
  }

}
