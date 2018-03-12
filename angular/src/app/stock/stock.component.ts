import { Component, OnInit, Input, ViewChild } from '@angular/core';
import { BaseChartDirective } from 'ng2-charts/ng2-charts';

import Stock from '../models/stock.model';
import Spot from '../models/spot.model';

import { StockService } from '../services/stock.service';
import { SpotService } from '../services/spot.service';

@Component({
  selector: 'app-stock',
  templateUrl: './stock.component.html',
  styleUrls: ['./stock.component.scss']
})
export class StockComponent implements OnInit {

  @Input() stockSymbol: string;

  public stock: Stock;
  public spots: Array<Spot>;

  @ViewChild('baseChart') chart: BaseChartDirective;

  public spotPrices: Array<Number> = [];
  public chartData: Array<any> = [{ data: this.spotPrices, label: '' }];
  public chartLabels: Array<any> = [];

  public chartOptions: any = {
    responsive: true,
    elements: {
      point: {
        radius: 1
      }
    }
  };
  public chartLegend = false;
  public chartType = 'line';

  constructor(
    private stockService: StockService,
    private spotService: SpotService
  ) { }

  public ngOnInit(): void {
    this.stockService.getStocks().subscribe(stocks => {
      // Find stock
      this.stock = stocks.find(stock => stock.symbol === this.stockSymbol);
      this.spots = [];

      // Chart label
      this.chartData[0].label = this.stock.name;

      this.spotService.getStockSpots(this.stock._id, 1, 250).subscribe(
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
            this.chartLabels.push(new Date(spot.date).toLocaleDateString());
            this.spotPrices.push(spot.price);
          });

          // Refresh chart
          this.chart.ngOnInit();
        }
      );
    });
  }

  public chartClicked(event: any): void {
    console.log(event);
  }

  public chartHovered(event: any): void {
    console.log(event);
  }

}
