import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { FinanceDataComponent } from './finance-data.component';

describe('FinanceDataComponent', () => {
  let component: FinanceDataComponent;
  let fixture: ComponentFixture<FinanceDataComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ FinanceDataComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(FinanceDataComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
