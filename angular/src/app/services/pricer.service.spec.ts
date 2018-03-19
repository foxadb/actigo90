import { TestBed, inject } from '@angular/core/testing';

import { PricerService } from './pricer.service';

describe('PricerService', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [PricerService]
    });
  });

  it('should be created', inject([PricerService], (service: PricerService) => {
    expect(service).toBeTruthy();
  }));
});
