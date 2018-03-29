import Stock from '../models/stock.model';
import Spot from '../models/spot.model';
import Price from '../models/price.model';
import Delta from '../models/delta.model';

import { Component, OnInit } from '@angular/core';
import { forkJoin } from 'rxjs/observable/forkJoin';

import { StockService } from '../services/stock.service';
import { SpotService } from '../services/spot.service';
import { DeltaService } from '../services/delta.service';
import { PriceService } from '../services/price.service';
import { PricerService } from '../services/pricer.service';

@Component({
  selector: 'app-pricing',
  templateUrl: './pricing.component.html',
  styleUrls: ['./pricing.component.scss']
})
export class PricingComponent implements OnInit {

  public euroStoxx50: Stock;
  public snp500: Stock;
  public snp200: Stock;
  public eurUsd: Stock;
  public eurAud: Stock;

  public euroStoxx50Spots: Array<Spot>;
  public snp500Spots: Array<Spot>;
  public snp200Spots: Array<Spot>;
  public eurUsdSpots: Array<Spot>;
  public eurAudSpots: Array<Spot>;

  public prices: Array<Price> = [];

  public deltas: Array<Delta>;
  public euroStoxx50Deltas: Array<Delta> = [];
  public snp500Deltas: Array<Delta> = [];
  public snp200Deltas: Array<Delta> = [];
  public eurUsdDeltas: Array<Delta> = [];
  public eurAudDeltas: Array<Delta> = [];

  // Number of Monte Carlo Samples
  public mcSamples = 5000;

  public pricingDate: Date;
  public pricing: any;

  // Spinners
  public refreshDataSpinner = false;
  public rebalancingSpinner = false;
  public pricingSpinner = false;

  constructor(
    private stockService: StockService,
    private spotService: SpotService,
    private deltaService: DeltaService,
    private priceService: PriceService,
    private pricerService: PricerService
  ) { }

  public ngOnInit(): void {
    this.stockService.getStocks().subscribe(
      stocks => {
        stocks.forEach(stock => {
          switch (stock.symbol) {
            case '^STOXX50E':
              this.euroStoxx50 = stock;
              break;
            case '^GSPC':
              this.snp500 = stock;
              break;
            case '^AXJO':
              this.snp200 = stock;
              break;
            case 'EURUSD=X':
              this.eurUsd = stock;
              break;
            case 'EURAUD=X':
              this.eurAud = stock;
              break;
          }
        });
      },
      error => console.error('Error', error),
      () => {
        // Get Deltas and Prices
        this.getDeltasPrices();
      }
    );
  }

  public getStockSpots(): void {
    const euroStoxx50Sub = this.spotService.getStockSpots(this.euroStoxx50._id, 1, 5000);
    const snp500Sub = this.spotService.getStockSpots(this.snp500._id, 1, 5000);
    const snp200Sub = this.spotService.getStockSpots(this.snp200._id, 1, 5000);
    const eurUsdSub = this.spotService.getStockSpots(this.eurUsd._id, 1, 5000);
    const eurAudSub = this.spotService.getStockSpots(this.eurAud._id, 1, 5000);

    forkJoin([euroStoxx50Sub, snp500Sub, snp200Sub, eurUsdSub, eurAudSub]).subscribe(
      spots => {
        // Get spots from database
        this.euroStoxx50Spots = spots[0];
        this.snp500Spots = spots[1];
        this.snp200Spots = spots[2];
        this.eurUsdSpots = spots[3];
        this.eurAudSpots = spots[4];

        // Sort spots by date
        this.sortByDate(this.euroStoxx50Spots);
        this.sortByDate(this.snp500Spots);
        this.sortByDate(this.snp200Spots);
        this.sortByDate(this.eurUsdSpots);
        this.sortByDate(this.eurAudSpots);
      });
 }

  public getDeltasPrices(): void {
    // Loading spinner
    this.refreshDataSpinner = true;

    this.priceService.getPrices(1, 15000).subscribe(
      prices => {
        this.prices = prices;
        this.sortByDate(this.prices);
      });

    this.deltaService.getDeltas(1, 15000).subscribe(
      deltas => {
        this.deltas = deltas;
        this.sortByDate(this.deltas);

        // Divide delta list into each stock
        this.deltas.forEach(delta => {
          switch (delta.stock) {
            case this.euroStoxx50._id:
              this.euroStoxx50Deltas.push(delta);
              break;
            case this.snp500._id:
              this.snp500Deltas.push(delta);
              break;
            case this.snp200._id:
              this.snp200Deltas.push(delta);
              break;
            case this.eurUsd._id:
              this.eurUsdDeltas.push(delta);
              break;
            case this.eurAud._id:
              this.eurAudDeltas.push(delta);
              break;
          }
        });

        // Stop loading spinner
        this.refreshDataSpinner = false;
      }
    );
  }

  public rebalance(): void {
    const body = {
      date: new Date(Date.now()).setUTCHours(0, 0, 0, 0) / 1000,
      samples: this.mcSamples
    };

    // Loading spinner
    this.rebalancingSpinner = true;

    // Rebalancing
    this.pricerService.rebalance(body).subscribe(
      res => {
        // Reload deltas and prices
        this.getDeltasPrices();

        // Stop spinner
        this.rebalancingSpinner = false;
      }
    );
  }

  public actigoDelta(): void {
    const body = {
      date: new Date(this.pricingDate).getTime() / 1000
    };

    // Loading spinner
    this.pricingSpinner = true;

    // Compute Actigo Delta
    this.pricerService.actigoDelta(body).subscribe(
      res => {
        this.pricing = res;
        this.pricingSpinner = false;
      },
      err => console.error('Error', err)
    );
  }

  public sortByDate(array: Array<any>): void {
    array.sort((a, b) =>
      new Date(b.date).getTime() - new Date(a.date).getTime());
  }

}
