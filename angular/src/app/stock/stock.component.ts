import { Component, OnInit, Input, ViewChild } from '@angular/core';
import { NgModel } from '@angular/forms';
import { BaseChartDirective } from 'ng2-charts/ng2-charts';
import { Subscription } from 'rxjs/Subscription';

import Stock from '../models/stock.model';
import Spot from '../models/spot.model';

import { StockService } from '../services/stock.service';
import { SpotService } from '../services/spot.service';
import { YahooFinanceService } from '../services/yahoo-finance.service';
import { FinancialDataService } from '../services/financial-data.service';

@Component({
  selector: 'app-stock',
  templateUrl: './stock.component.html',
  styleUrls: ['./stock.component.scss']
})
export class StockComponent implements OnInit {

  @Input() stockSymbol: string;

  public stock: Stock;
  public spots: Array<Spot>;
  public lastSpot: Spot;

  public startDate: Date;
  public endDate = Date.now();

  public selectedDate: Date;
  public selectedSpot: Spot;

  @ViewChild('baseChart') chart: BaseChartDirective;

  public spotPrices: Array<Number> = [];
  public chartData: Array<any> = [{ data: this.spotPrices, label: '' }];
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
  public chartLegend = false;
  public chartType = 'line';

  public downloadSpinner = false;

  private parentDownloadSubscription: Subscription;
  private parentDeleteSubscription: Subscription;

  constructor(
    private stockService: StockService,
    private spotService: SpotService,
    private yahooFinanceService: YahooFinanceService,
    private financialDataService: FinancialDataService
  ) { }

  public ngOnInit(): void {
    // Stock subscription
    this.stockService.getStocks().subscribe(stocks => {
      // Find stock
      this.stock = stocks.find(stock => stock.symbol === this.stockSymbol);

      // Get Spots
      this.getStockSpots(this.stock._id, 1, 5000);

    });

    // Parent download data request subscription
    this.parentDownloadSubscription = this.financialDataService.downloadObservable.subscribe(
      res => {
        // Set start and end date
        this.startDate = res.startDate;
        this.endDate = res.endDate;

        // Download data
        this.downloadStockData();
      }
    );

    // Parend delete data request subscription
    this.parentDeleteSubscription = this.financialDataService.deleteObservable.subscribe(
      res => {
        if (res === true) {
          // Delete data
          this.deleteStockData();
        }
      }
    );
  }

  public getStockSpots(stock: string, page: number, limit: number): void {
    // Reset chart labels and prices
    this.chartLabels.length = 0;
    this.spotPrices.length = 0;

    this.spotService.getStockSpots(stock, page, limit).subscribe(
      spots => {
        this.spots = spots;
      },
      error => console.error('Error: ', error),
      () => {
        // Sort spot by date
        this.spots.sort((a, b) =>
          new Date(a.date).getTime() - new Date(b.date).getTime());

        // Fill chart data
        this.spots.forEach(spot => {
          this.chartLabels.push(new Date(spot.date));
          this.spotPrices.push(spot.price);
        });

        // Init last and selected spots
        this.lastSpot = this.spots[this.spots.length - 1];
        this.selectedSpot = this.lastSpot;
        this.selectedDate = this.selectedSpot.date;

        // Refresh chart
        this.chart.ngOnInit();
      }
    );
  }

  public downloadStockData(): void {
    if (this.startDate && this.endDate) {
      const data = {
        stock: this.stock._id,
        period1: Math.round(new Date(this.startDate).getTime() / 1000),
        period2: Math.round(new Date(this.endDate).getTime() / 1000),
        interval: '1d'
      };

      // Start loading spinner
      this.downloadSpinner = true;

      this.yahooFinanceService.downloadStockData(data).subscribe(
        res => {
          // Get Spots
          this.getStockSpots(this.stock._id, 1, 5000);

          // Stop loading spinner
          this.downloadSpinner = false;
        },
        err => {
          // Retrying to download data
          this.downloadStockData();
        });
    }
  }

  public deleteStockData(): void {
    this.spotService.deleteStockSpots(this.stock._id).subscribe(
      res => {
        // Reset chart labels and prices
        this.chartLabels.length = 0;
        this.spotPrices.length = 0;

        // Refresh chart
        this.chart.ngOnInit();
      });
  }

  public chartClicked(event: any): void {
    console.log(event);
  }

  public chartHovered(event: any): void {
    console.log(event);
  }

  public showSelectedSpot(): void {
    this.selectedSpot = this.spots.find(spot =>
      new Date(spot.date).getTime() === new Date(this.selectedDate).getTime()
    );
  }

}
