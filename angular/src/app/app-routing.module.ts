import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';

import { HomeComponent } from './home/home.component';
import { LoginComponent } from './login/login.component';
import { PricingComponent } from './pricing/pricing.component';
import { FinancialDataComponent } from './financial-data/financial-data.component';
import { ContactComponent } from './contact/contact.component';

import { AuthGuard } from './services/auth.guard';

const routes: Routes = [
  // Home page
  { path: '', component: HomeComponent },

  // Login page
  { path: 'login', component: LoginComponent },

  // Pricing page
  {
    path: 'pricing',
    component: PricingComponent,
    canActivate: [AuthGuard],
    data: { expectedRole: 'user' }
  },

  // Financial data page
  {
    path: 'financial-data',
    component: FinancialDataComponent,
    canActivate: [AuthGuard],
    data: { expectedRole: 'user' }
  },

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
