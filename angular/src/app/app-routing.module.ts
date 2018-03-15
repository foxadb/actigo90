import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';

import { HomeComponent } from './home/home.component';
import { PricingComponent } from './pricing/pricing.component';
import { FinanceDataComponent } from './finance-data/finance-data.component';
import { ContactComponent } from './contact/contact.component';

const routes: Routes = [
  // Home page
  { path: '', component: HomeComponent },

  // Pricing page
  { path: 'pricing', component: PricingComponent },

  // Player stats page
  { path: 'finance-data', component: FinanceDataComponent },

  // Contact page
  { path: 'contact', component: ContactComponent },

  // Redirect other path to home
  { path: '**', redirectTo: '' }
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
