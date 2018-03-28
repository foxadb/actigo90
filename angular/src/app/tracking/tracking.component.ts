import { Component, OnInit, ViewChild } from '@angular/core';
import { BaseChartDirective } from 'ng2-charts/ng2-charts';

import Price from '../models/price.model';

import { DeltaService } from '../services/delta.service';
import { PriceService } from '../services/price.service';
import { PricerService } from '../services/pricer.service';

@Component({
  selector: 'app-tracking',
  templateUrl: './tracking.component.html',
  styleUrls: ['./tracking.component.scss']
})
export class TrackingComponent implements OnInit {

  public hedgingDate: Date;
  public hedgingSpinner = false;

  // Rebalancing Frequency in days
  public rebalancingFrequency = 1;

  // Number of Monte Carlo Samples
  public mcSamples = 5000;

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
    },
    scales: {
      xAxes: [{
        type: 'time',
        time: {
          displayFormats: {
            quarter: 'MMM YYYY'
          }
        }
      }]
    },
    pan: {
      enabled: true,
      mode: 'x'
    },
    zoom: {
      enabled: true,
      mode: 'x',
    }
  };
  public chartLegend = true;
  public chartType = 'line';

  constructor(
    private deltaService: DeltaService,
    private priceService: PriceService,
    private pricerService: PricerService
  ) { }

  public ngOnInit(): void {
    this.refreshChartData();
  }

  public refreshChartData(): void {
    // Download chart data
    this.getChartData();
  }

  public getChartData(): void {
    // Reset chart labels and prices
    this.chartLabels.length = 0;
    this.actigoPrices.length = 0;
    this.hedgingPrices.length = 0;

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
          this.chartLabels.push(new Date(price.date));
          this.actigoPrices.push(price.actigo);
          this.hedgingPrices.push(price.hedging);
        });

        // Refresh chart
        this.chart.ngOnInit();
      }
    );
  }

  public hedging(): void {
    const body = {
      date: new Date(this.hedgingDate).getTime() / 1000,
      frequency: this.rebalancingFrequency,
      samples: this.mcSamples
    };

    // Loading
    this.hedgingSpinner = true;

    // Compute Actigo Delta
    this.pricerService.hedging(body).subscribe(
      res => {
        this.hedgingSpinner = false;
      },
      err => console.error('Error', err)
    );
  }

  public deleteHedging(): void {
    // Loading spinner
    this.hedgingSpinner = true;

    // Delete all deltas and prices
    this.deltaService.deleteAllDeltas().subscribe(
      deltas => {
        this.priceService.deleteAllPrices().subscribe(
          prices => {
            this.getChartData();
            this.hedgingSpinner = false;
          }
        );
      }
    );
  }

  public computeTrackingError(): void {
    // Compute error serie and average error
    const errorsSerie = [];
    let averageError = 0;
    let i = 1;

    while (new Date(this.prices[i].date).getTime()
      <= new Date(this.trackingDate).getTime()) {
      // Get current and previous prices
      const currentPrice = this.prices[i];
      const previousPrice = this.prices[i - 1];

      // Compute porfolio performances (percent)
      const perfActigo = 100 * (currentPrice.actigo / previousPrice.actigo - 1);
      const perfHedging = 100 * (currentPrice.hedging / previousPrice.hedging - 1);

      // Compute current error and add it to the list
      const error = perfHedging - perfActigo;
      errorsSerie.push(error);

      // Add error to average
      averageError += error;

      // Skip to next day
      ++i;
    }

    // Compute average error
    averageError /= i - 1;

    // Compute tracking error (std dev)
    let trackingError = 0;
    errorsSerie.forEach(error => trackingError += (error - averageError) ** 2);
    trackingError /= i - 1;
    trackingError = Math.sqrt(trackingError);
    this.trackingError = trackingError;
  }

  public chartClicked(event: any): void {
    console.log(event);
  }

  public chartHovered(event: any): void {
    console.log(event);
  }

}
