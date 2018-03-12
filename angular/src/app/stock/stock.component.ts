import { Component, OnInit, Input } from '@angular/core';

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

  stock: Stock;
  spots: Array<Spot>;

  constructor(
    private stockService: StockService,
    private spotService: SpotService
  ) { }

  ngOnInit() {
    this.stockService.getStocks().subscribe(stocks => {
      // Find stock
      this.stock = stocks.find(stock => stock.symbol === this.stockSymbol);
      this.spots = [];

      // Retreive spots
      this.stock.spots.forEach(id => {
        this.spotService.getSpot(id).subscribe(
          spot => {
            this.spots.push(spot);
          },
          error => console.log('Error: ', error),
          () => {
            // Sort spot by date
            this.spots.sort((a, b) =>
              new Date(b.date).getTime() - new Date(a.date).getTime());
          });
      });
    });
  }

}
