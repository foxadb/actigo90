import { TestBed, inject } from '@angular/core/testing';

import { YahooFinanceService } from './yahoo-finance.service';

describe('YahooFinanceService', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [YahooFinanceService]
    });
  });

  it('should be created', inject([YahooFinanceService], (service: YahooFinanceService) => {
    expect(service).toBeTruthy();
  }));
});
