import { Component, OnInit, Input, ViewChild } from '@angular/core';
import { NgModel } from '@angular/forms';
import { BaseChartDirective } from 'ng2-charts/ng2-charts';

import Stock from '../models/stock.model';
import Spot from '../models/spot.model';

import { StockService } from '../services/stock.service';
import { SpotService } from '../services/spot.service';
import { YahooFinanceService } from '../services/yahoo-finance.service';

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
    }
  };
  public chartLegend = false;
  public chartType = 'line';

  public downloadSpinner = false;

  constructor(
    private stockService: StockService,
    private spotService: SpotService,
    private yahooFinanceService: YahooFinanceService
  ) { }

  public ngOnInit(): void {
    this.stockService.getStocks().subscribe(stocks => {
      // Find stock
      this.stock = stocks.find(stock => stock.symbol === this.stockSymbol);

      // Get Spots
      this.getStockSpots(this.stock._id, 1, 500);
    });
  }

  public getStockSpots(stock: string, page: number, limit: number): void {
    // Reset chart labels and prices
    this.chartLabels.length = 0;
    this.spotPrices.length = 0;

    this.spotService.getStockSpots(stock, page, limit).subscribe(
      spots => {
        this.spots = spots;
        this.lastSpot = spots[spots.length - 1];
      },
      error => console.error('Error: ', error),
      () => {
        // Sort spot by date
        this.spots.sort((a, b) =>
          new Date(a.date).getTime() - new Date(b.date).getTime());

        // Fill chart data
        this.spots.forEach(spot => {
          this.chartLabels.push(new Date(spot.date).toLocaleDateString());
          this.spotPrices.push(spot.price);
        });

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
          this.getStockSpots(this.stock._id, 1, 1000);

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

}
