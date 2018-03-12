import { Component } from '@angular/core';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent {
  title = 'Actigo 90';
  euroStoxx50 = '^STOXX50E';
  snp500 = '^GSPC';
  snp200 = '^AXJO';
}
