import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { NgbModule } from '@ng-bootstrap/ng-bootstrap';
import { HttpClientModule } from '@angular/common/http';
import { ChartsModule } from 'ng2-charts/ng2-charts';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { HomeComponent } from './home/home.component';
import { FinanceDataComponent } from './finance-data/finance-data.component';
import { StockComponent } from './stock/stock.component';
import { PricingComponent } from './pricing/pricing.component';
import { ContactComponent } from './contact/contact.component';

import { StockService } from './services/stock.service';
import { SpotService } from './services/spot.service';

@NgModule({
  declarations: [
    AppComponent,
    HomeComponent,
    FinanceDataComponent,
    StockComponent,
    PricingComponent,
    ContactComponent
  ],
  imports: [
    NgbModule.forRoot(),
    BrowserModule,
    HttpClientModule,
    ChartsModule,
    AppRoutingModule
  ],
  providers: [
    StockService,
    SpotService
  ],
  bootstrap: [AppComponent]
})
export class AppModule { }
