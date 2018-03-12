import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { NgbModule } from '@ng-bootstrap/ng-bootstrap';
import { HttpClientModule } from '@angular/common/http';
import { ChartsModule } from 'ng2-charts/ng2-charts';

import { AppComponent } from './app.component';
import { StockComponent } from './stock/stock.component';

import { StockService } from './services/stock.service';
import { SpotService } from './services/spot.service';

@NgModule({
  declarations: [
    AppComponent,
    StockComponent
  ],
  imports: [
    NgbModule.forRoot(),
    BrowserModule,
    HttpClientModule,
    ChartsModule
  ],
  providers: [
    StockService,
    SpotService
  ],
  bootstrap: [AppComponent]
})
export class AppModule { }
