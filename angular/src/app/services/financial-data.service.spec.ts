import { TestBed, inject } from '@angular/core/testing';

import { FinancialDataService } from './financial-data.service';

describe('FinancialDataService', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [FinancialDataService]
    });
  });

  it('should be created', inject([FinancialDataService], (service: FinancialDataService) => {
    expect(service).toBeTruthy();
  }));
});
