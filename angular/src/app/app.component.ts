import { Component } from '@angular/core';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent {
  public title = 'Actigo 90';
  public euroStoxx50 = '^STOXX50E';
  public snp500 = '^GSPC';
  public snp200 = '^AXJO';
}
